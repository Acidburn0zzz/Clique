/*
   Clique: a scalable implementation of the multifrontal algorithm

   Copyright (C) 2011-2012 Jack Poulson, Lexing Ying, and 
   The University of Texas at Austin
 
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
 
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "clique.hpp"
using namespace cliq;

void Usage()
{
    std::cout
      << "Refactor <n1> <n2> <n3> <numRepeats> "
      << "[sequential=true] [numDistSeps=1] [numSeqSeps=1] [cutoff=128]\n"
      << "  n1: first dimension of n1 x n2 x n3 mesh\n"
      << "  n2: second dimension of n1 x n2 x n3 mesh\n"
      << "  n3: third dimension of n1 x n2 x n3 mesh\n"
      << "  numRepeats: number of times to modify and refactor\n"
      << "  sequential: if nonzero, then run a sequential symbolic reordering\n"
      << "  numDistSeps: number of distributed separators to try\n"
      << "  numSeqSeps: number of sequential separators to try\n"
      << "  cutoff: maximum size of leaf node\n"
      << std::endl;
}

int
main( int argc, char* argv[] )
{
    cliq::Initialize( argc, argv );
    mpi::Comm comm = mpi::COMM_WORLD;
    const int commRank = mpi::CommRank( comm );

    if( argc < 5 )
    {
        if( commRank == 0 )
            Usage();
        cliq::Finalize();
        return 0;
    }
    int argNum = 1;
    const int n1 = atoi(argv[argNum++]);
    const int n2 = atoi(argv[argNum++]);
    const int n3 = atoi(argv[argNum++]);
    const int numRepeats = atoi(argv[argNum++]);
    const bool sequential = ( argc>argNum ? atoi(argv[argNum++]) : true );
    const int numDistSeps = ( argc>argNum ? atoi(argv[argNum++]) : 1 );
    const int numSeqSeps = ( argc>argNum ? atoi(argv[argNum++]) : 1 );
    const int cutoff = ( argc>argNum ? atoi(argv[argNum++]) : 128 );

    try
    {
        const int N = n1*n2*n3;
        DistSparseMatrix<double> A( N, comm );

        // Fill our portion of the 3D negative Laplacian using a n1 x n2 x n3
        // 7-point stencil in natural ordering: (x,y,z) at x + y*n1 + z*n1*n2
        if( commRank == 0 )
        {
            std::cout << "Filling local portion of matrix...";
            std::cout.flush();
        }
        const double fillStart = mpi::Time();
        const int firstLocalRow = A.FirstLocalRow();
        const int localHeight = A.LocalHeight();
        A.StartAssembly();
        A.Reserve( 7*localHeight );
        for( int iLocal=0; iLocal<localHeight; ++iLocal )
        {
            const int i = firstLocalRow + iLocal;
            const int x = i % n1;
            const int y = (i/n1) % n2;
            const int z = i/(n1*n2);

            A.Update( i, i, 6. );
            if( x != 0 )
                A.Update( i, i-1, -1. );
            if( x != n1-1 )
                A.Update( i, i+1, -1. );
            if( y != 0 )
                A.Update( i, i-n1, -1. );
            if( y != n2-1 )
                A.Update( i, i+n1, -1. );
            if( z != 0 )
                A.Update( i, i-n1*n2, -1. );
            if( z != n3-1 )
                A.Update( i, i+n1*n2, -1. );
        } 
        A.StopAssembly();
        mpi::Barrier( comm );
        const double fillStop =  mpi::Time();
        if( commRank == 0 )
            std::cout << "done, " << fillStop-fillStart << " seconds" 
                      << std::endl;

        if( commRank == 0 )
        {
            std::cout << "Running nested dissection...";
            std::cout.flush();
        }
        const double nestedStart = mpi::Time();
        const DistGraph& graph = A.Graph();
        DistSymmInfo info;
        DistSeparatorTree sepTree;
        DistMap map, inverseMap;
        NestedDissection
        ( graph, map, sepTree, info, 
          sequential, numDistSeps, numSeqSeps, cutoff );
        map.FormInverse( inverseMap );
        mpi::Barrier( comm );
        const double nestedStop = mpi::Time();
        if( commRank == 0 )
            std::cout << "done, " << nestedStop-nestedStart << " seconds"
                      << std::endl;

        if( commRank == 0 )
        {
            const int numDistNodes = info.distNodes.size();
            const int numLocalNodes = info.localNodes.size();
            const int rootSepSize = info.distNodes.back().size;
            std::cout << "\n"
                      << "On the root process:\n"
                      << "-----------------------------------------\n"
                      << numLocalNodes << " local nodes\n"
                      << numDistNodes  << " distributed nodes\n"
                      << rootSepSize << " vertices in root separator\n"
                      << std::endl;
        }

        if( commRank == 0 )
        {
            std::cout << "Building DistSymmFrontTree...";
            std::cout.flush();
        }
        mpi::Barrier( comm );
        const double buildStart = mpi::Time();
        DistSymmFrontTree<double> frontTree( TRANSPOSE, A, map, sepTree, info );
        mpi::Barrier( comm );
        const double buildStop = mpi::Time();
        if( commRank == 0 )
            std::cout << "done, " << buildStop-buildStart << " seconds"
                      << std::endl;

        for( int repeat=0; repeat<numRepeats; ++repeat )
        {
            if( repeat != 0 )
            {
                // Reset to an unfactored, implicitly symmetric frontal tree
                if( commRank == 0 )
                    std::cout << "Resetting frontal tree." << std::endl;
                ChangeFrontType( frontTree, SYMM_2D );

                // Randomize the fronts
                if( commRank == 0 )
                    std::cout << "Randomizing fronts." << std::endl;
                const int numDistFronts = frontTree.distFronts.size();
                const int numLocalFronts = frontTree.localFronts.size();
                for( int s=0; s<numLocalFronts; ++s )
                    elem::MakeUniform( frontTree.localFronts[s].frontL );
                for( int s=1; s<numDistFronts; ++s )
                    elem::MakeUniform( frontTree.distFronts[s].front2dL );
            }

            if( commRank == 0 )
            {
                std::cout << "Running LDL^T and redistribution...";
                std::cout.flush();
            }
            mpi::Barrier( comm );
            const double ldlStart = mpi::Time();
            LDL( info, frontTree, LDL_1D );
            mpi::Barrier( comm );
            const double ldlStop = mpi::Time();
            if( commRank == 0 )
                std::cout << "done, " << ldlStop-ldlStart << " seconds" 
                          << std::endl;

            if( commRank == 0 )
            {
                std::cout << "Solving against random right-hand side...";
                std::cout.flush();
            }
            const double solveStart = mpi::Time();
            DistVector<double> y( N, comm );
            MakeUniform( y );
            DistNodalVector<double> yNodal;
            yNodal.Pull( inverseMap, info, y );
            Solve( info, frontTree, yNodal.localVec );
            yNodal.Push( inverseMap, info, y );
            mpi::Barrier( comm );
            const double solveStop = mpi::Time();
            if( commRank == 0 )
                std::cout << "done, " << solveStop-solveStart << " seconds"
                          << std::endl;

            // TODO: Check residual error
        }
    }
    catch( std::exception& e )
    {
#ifndef RELEASE
        elem::DumpCallStack();
        cliq::DumpCallStack();
#endif
        std::ostringstream msg;
        msg << "Process " << commRank << " caught message:\n"
            << e.what() << "\n";
        std::cerr << msg.str() << std::endl;
    }

    cliq::Finalize();
    return 0;
}
