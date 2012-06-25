/*
   Clique: a scalable implementation of the multifrontal algorithm

   Copyright (C) 2011 Jack Poulson, Lexing Ying, and 
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
#ifndef CLIQUE_NUMERIC_DIST_LOWER_SOLVE_HPP
#define CLIQUE_NUMERIC_DIST_LOWER_SOLVE_HPP 1

namespace cliq {
namespace numeric {

template<typename F> 
void DistLowerForwardSolve
( UnitOrNonUnit diag,
  const symbolic::SymmFact& S,
  const numeric::SymmFrontTree<F>& L, 
        Matrix<F>& localX );

template<typename F>
void DistLowerBackwardSolve
( Orientation orientation, UnitOrNonUnit diag,
  const symbolic::SymmFact& S,
  const numeric::SymmFrontTree<F>& L,
        Matrix<F>& localX );

//----------------------------------------------------------------------------//
// Implementation begins here                                                 //
//----------------------------------------------------------------------------//

template<typename F> 
inline void DistLowerForwardSolve
( UnitOrNonUnit diag,
  const symbolic::SymmFact& S,
  const numeric::SymmFrontTree<F>& L, 
        Matrix<F>& localX )
{
    using namespace symbolic;
#ifndef RELEASE
    PushCallStack("numeric::DistLowerForwardSolve");
#endif
    const int numDistSupernodes = S.dist.supernodes.size();
    const int width = localX.Width();
    const SolveMode mode = L.dist.mode;
    const bool modeIs1d = ModeIs1d( mode );
    if( mode != NORMAL_1D && mode != FAST_1D_LDL && mode != FAST_2D_LDL )
        throw std::logic_error("This solve mode is not yet implemented");

    // Copy the information from the local portion into the distributed leaf
    const LocalSymmFront<F>& localRootFront = L.local.fronts.back();
    const DistSymmFront<F>& distLeafFront = L.dist.fronts[0];
    const Grid& leafGrid = ( modeIs1d ? distLeafFront.front1dL.Grid() 
                                      : distLeafFront.front2dL.Grid() );
    distLeafFront.work1d.LockedView
    ( localRootFront.work.Height(), localRootFront.work.Width(), 0,
      localRootFront.work.LockedBuffer(), localRootFront.work.LDim(), 
      leafGrid );
    
    // Perform the distributed portion of the forward solve
    for( int s=1; s<numDistSupernodes; ++s )
    {
        const DistSymmFactSupernode& childSN = S.dist.supernodes[s-1];
        const DistSymmFactSupernode& sn = S.dist.supernodes[s];
        const DistSymmFront<F>& childFront = L.dist.fronts[s-1];
        const DistSymmFront<F>& front = L.dist.fronts[s];
        const Grid& childGrid = ( modeIs1d ? childFront.front1dL.Grid()
                                           : childFront.front2dL.Grid() );
        const Grid& grid = ( modeIs1d ? front.front1dL.Grid()
                                      : front.front2dL.Grid() );
        mpi::Comm comm = grid.VCComm();
        mpi::Comm childComm = childGrid.VCComm();
        const int commSize = mpi::CommSize( comm );
        const int commRank = mpi::CommRank( comm );
        const int childCommSize = mpi::CommSize( childComm );
        const int frontHeight = ( modeIs1d ? front.front1dL.Height()
                                           : front.front2dL.Height() );

        // Set up a workspace
        DistMatrix<F,VC,STAR>& W = front.work1d;
        W.SetGrid( grid );
        W.ResizeTo( frontHeight, width );
        DistMatrix<F,VC,STAR> WT(grid), WB(grid);
        elem::PartitionDown
        ( W, WT,
             WB, sn.size );

        // Pull in the relevant information from the RHS
        Matrix<F> localXT;
        localXT.View( localX, sn.localOffset1d, 0, sn.localSize1d, width );
        WT.LocalMatrix() = localXT;
        elem::MakeZeros( WB );

        // Pack our child's update
        DistMatrix<F,VC,STAR>& childW = childFront.work1d;
        const int updateSize = childW.Height()-childSN.size;
        DistMatrix<F,VC,STAR> childUpdate;
        childUpdate.LockedView( childW, childSN.size, 0, updateSize, width );
        int sendBufferSize = 0;
        std::vector<int> sendCounts(commSize), sendDispls(commSize);
        for( int proc=0; proc<commSize; ++proc )
        {
            const int sendSize = sn.numChildSolveSendIndices[proc]*width;
            sendCounts[proc] = sendSize;
            sendDispls[proc] = sendBufferSize;
            sendBufferSize += sendSize;
        }
        std::vector<F> sendBuffer( sendBufferSize );

        const bool isLeftChild = ( commRank < commSize/2 );
        const std::vector<int>& myChildRelIndices = 
            ( isLeftChild ? sn.leftChildRelIndices
                          : sn.rightChildRelIndices );
        const int updateColShift = childUpdate.ColShift();
        const int updateLocalHeight = childUpdate.LocalHeight();
        std::vector<int> packOffsets = sendDispls;
        for( int iChildLocal=0; iChildLocal<updateLocalHeight; ++iChildLocal )
        {
            const int iChild = updateColShift + iChildLocal*childCommSize;
            const int destRank = myChildRelIndices[iChild] % commSize;
            F* packBuf = &sendBuffer[packOffsets[destRank]];
            for( int jChild=0; jChild<width; ++jChild )
                packBuf[jChild] = childUpdate.GetLocal(iChildLocal,jChild);
            packOffsets[destRank] += width;
        }
        packOffsets.clear();
        childW.Empty();
        if( s == 1 )
            L.local.fronts.back().work.Empty();

        // Set up the receive buffer
        int recvBufferSize = 0;
        std::vector<int> recvCounts(commSize), recvDispls(commSize);
        for( int proc=0; proc<commSize; ++proc )
        {
            const int recvSize = sn.childSolveRecvIndices[proc].size()*width;
            recvCounts[proc] = recvSize;
            recvDispls[proc] = recvBufferSize;
            recvBufferSize += recvSize;
        }
        std::vector<F> recvBuffer( recvBufferSize );
#ifndef RELEASE
        VerifySendsAndRecvs( sendCounts, recvCounts, comm );
#endif

        // AllToAll to send and receive the child updates
        SparseAllToAll
        ( sendBuffer, sendCounts, sendDispls,
          recvBuffer, recvCounts, recvDispls, comm );
        sendBuffer.clear();
        sendCounts.clear();
        sendDispls.clear();

        // Unpack the child updates (with an Axpy)
        for( int proc=0; proc<commSize; ++proc )
        {
            const F* recvValues = &recvBuffer[recvDispls[proc]];
            const std::deque<int>& recvIndices = sn.childSolveRecvIndices[proc];
            for( unsigned k=0; k<recvIndices.size(); ++k )
            {
                const int iFrontLocal = recvIndices[k];
                const F* recvRow = &recvValues[k*width];
                F* WRow = W.LocalBuffer( iFrontLocal, 0 );
                const int WLDim = W.LocalLDim();
                for( int j=0; j<width; ++j )
                    WRow[j*WLDim] += recvRow[j];
            }
        }
        recvBuffer.clear();
        recvCounts.clear();
        recvDispls.clear();

        // Now that the RHS is set up, perform this supernode's solve
        if( mode == NORMAL_1D )
            DistFrontLowerForwardSolve( diag, front.front1dL, W );
        else if( mode == FAST_1D_LDL )
            DistFrontFastLowerForwardSolve( diag, front.front1dL, W );
        else // mode == FAST_2D_LDL
            DistFrontFastLowerForwardSolve( diag, front.front2dL, W );

        // Store the supernode portion of the result
        localXT = WT.LocalMatrix();
    }
    L.local.fronts.back().work.Empty();
    L.dist.fronts.back().work1d.Empty();
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename F>
inline void DistLowerBackwardSolve
( Orientation orientation, UnitOrNonUnit diag,
  const symbolic::SymmFact& S,
  const numeric::SymmFrontTree<F>& L,
        Matrix<F>& localX )
{
    using namespace symbolic;
#ifndef RELEASE
    PushCallStack("numeric::DistLowerBackwardSolve");
#endif
    const int numDistSupernodes = S.dist.supernodes.size();
    const int width = localX.Width();
    const SolveMode mode = L.dist.mode;
    const bool modeIs1d = ModeIs1d( mode );
    if( mode != NORMAL_1D && mode != FAST_1D_LDL && mode != FAST_2D_LDL )
        throw std::logic_error("This solve mode is not yet implemented");

    // Directly operate on the root separator's portion of the right-hand sides
    const DistSymmFactSupernode& rootSN = S.dist.supernodes.back();
    const LocalSymmFront<F>& localRootFront = L.local.fronts.back();
    if( numDistSupernodes == 1 )
    {
        localRootFront.work.View
        ( rootSN.size, width, 
          localX.Buffer(rootSN.localOffset1d,0), localX.LDim() );
        LocalFrontLowerBackwardSolve
        ( orientation, diag, localRootFront.frontL, localRootFront.work );
    }
    else
    {
        const DistSymmFront<F>& rootFront = L.dist.fronts.back();
        const Grid& rootGrid = ( modeIs1d ? rootFront.front1dL.Grid() 
                                          : rootFront.front2dL.Grid() );
        rootFront.work1d.View
        ( rootSN.size, width, 0,
          localX.Buffer(rootSN.localOffset1d,0), localX.LDim(), rootGrid );
        if( mode == NORMAL_1D )
            DistFrontLowerBackwardSolve
            ( orientation, diag, rootFront.front1dL, rootFront.work1d );
        else if( mode == FAST_1D_LDL )
            DistFrontFastLowerBackwardSolve
            ( orientation, diag, rootFront.front1dL, rootFront.work1d );
        else // mode == FAST_2D_LDL
            DistFrontFastLowerBackwardSolve
            ( orientation, diag, rootFront.front2dL, rootFront.work1d );
    }

    for( int s=numDistSupernodes-2; s>=0; --s )
    {
        const DistSymmFactSupernode& parentSN = S.dist.supernodes[s+1];
        const DistSymmFactSupernode& sn = S.dist.supernodes[s];
        const DistSymmFront<F>& parentFront = L.dist.fronts[s+1];
        const DistSymmFront<F>& front = L.dist.fronts[s];
        const Grid& grid = ( modeIs1d ? front.front1dL.Grid() 
                                      : front.front2dL.Grid() );
        const Grid& parentGrid = ( modeIs1d ? parentFront.front1dL.Grid()
                                            : parentFront.front2dL.Grid() );
        mpi::Comm comm = grid.VCComm(); 
        mpi::Comm parentComm = parentGrid.VCComm();
        const int commSize = mpi::CommSize( comm );
        const int parentCommSize = mpi::CommSize( parentComm );
        const int parentCommRank = mpi::CommRank( parentComm );
        const int frontHeight = ( modeIs1d ? front.front1dL.Height()
                                           : front.front2dL.Height() );

        // Set up a workspace
        DistMatrix<F,VC,STAR>& W = front.work1d;
        W.SetGrid( grid );
        W.ResizeTo( frontHeight, width );
        DistMatrix<F,VC,STAR> WT(grid), WB(grid);
        elem::PartitionDown
        ( W, WT,
             WB, sn.size );

        // Pull in the relevant information from the RHS
        Matrix<F> localXT;
        localXT.View( localX, sn.localOffset1d, 0, sn.localSize1d, width );
        WT.LocalMatrix() = localXT;

        //
        // Set the bottom from the parent
        //

        // Pack the updates using the recv approach from the forward solve
        int sendBufferSize = 0;
        std::vector<int> sendCounts(parentCommSize), sendDispls(parentCommSize);
        for( int proc=0; proc<parentCommSize; ++proc )
        {
            const int sendSize = 
                parentSN.childSolveRecvIndices[proc].size()*width;
            sendCounts[proc] = sendSize;
            sendDispls[proc] = sendBufferSize;
            sendBufferSize += sendSize;
        }
        std::vector<F> sendBuffer( sendBufferSize );

        DistMatrix<F,VC,STAR>& parentWork = parentFront.work1d;
        for( int proc=0; proc<parentCommSize; ++proc )
        {
            F* sendValues = &sendBuffer[sendDispls[proc]];
            const std::deque<int>& recvIndices = 
                parentSN.childSolveRecvIndices[proc];
            for( unsigned k=0; k<recvIndices.size(); ++k )
            {
                const int iFrontLocal = recvIndices[k];
                F* sendRow = &sendValues[k*width];
                const F* workRow = 
                    parentWork.LockedLocalBuffer( iFrontLocal, 0 );
                const int workLDim = parentWork.LocalLDim();
                for( int j=0; j<width; ++j )
                    sendRow[j] = workRow[j*workLDim];
            }
        }
        parentWork.Empty();

        // Set up the receive buffer
        int recvBufferSize = 0;
        std::vector<int> recvCounts(parentCommSize), recvDispls(parentCommSize);
        for( int proc=0; proc<parentCommSize; ++proc )
        {
            const int recvSize = 
                parentSN.numChildSolveSendIndices[proc]*width;
            recvCounts[proc] = recvSize;
            recvDispls[proc] = recvBufferSize;
            recvBufferSize += recvSize;
        }
        std::vector<F> recvBuffer( recvBufferSize );
#ifndef RELEASE
        VerifySendsAndRecvs( sendCounts, recvCounts, parentComm );
#endif

        // AllToAll to send and recv parent updates
        SparseAllToAll
        ( sendBuffer, sendCounts, sendDispls,
          recvBuffer, recvCounts, recvDispls, parentComm );
        sendBuffer.clear();
        sendCounts.clear();
        sendDispls.clear();

        // Unpack the updates using the send approach from the forward solve
        const bool isLeftChild = ( parentCommRank < parentCommSize/2 );
        const std::vector<int>& myRelIndices = 
            ( isLeftChild ? parentSN.leftChildRelIndices
                          : parentSN.rightChildRelIndices );
        const int updateColShift = WB.ColShift();
        const int updateLocalHeight = WB.LocalHeight();
        for( int iUpdateLocal=0; 
                 iUpdateLocal<updateLocalHeight; ++iUpdateLocal )
        {
            const int iUpdate = updateColShift + iUpdateLocal*commSize;
            const int startRank = myRelIndices[iUpdate] % parentCommSize;
            const F* recvBuf = &recvBuffer[recvDispls[startRank]];
            for( int j=0; j<width; ++j )
                WB.SetLocal(iUpdateLocal,j,recvBuf[j]);
            recvDispls[startRank] += width;
        }
        recvBuffer.clear();
        recvCounts.clear();
        recvDispls.clear();

        // Call the custom supernode backward solve
        if( s > 0 )
        {
            if( mode == NORMAL_1D )
                DistFrontLowerBackwardSolve
                ( orientation, diag, front.front1dL, W );
            else if( mode == FAST_1D_LDL )
                DistFrontFastLowerBackwardSolve
                ( orientation, diag, front.front1dL, W );
            else // mode == FAST_2D_LDL
                DistFrontFastLowerBackwardSolve
                ( orientation, diag, front.front2dL, W );
        }
        else
        {
            LocalFrontLowerBackwardSolve
            ( orientation, diag, localRootFront.frontL, W.LocalMatrix() );
        }

        // Store the supernode portion of the result
        localXT = WT.LocalMatrix();
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

} // namespace numeric
} // namespace cliq

#endif // CLIQUE_NUMERIC_DIST_LOWER_SOLVE_HPP