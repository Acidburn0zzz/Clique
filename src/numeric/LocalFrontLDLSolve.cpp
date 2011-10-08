/*
   Modification of include/elemental/basic/level3/Trsm/TrsmLLN.hpp 
   from Elemental.
   Copyright (c) 2009-2011, Jack Poulson
   All rights reserved.

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
#include "clique.hpp"
using namespace elemental;

template<typename F>
void clique::numeric::LocalFrontLDLDiagonalSolve
( int supernodeSize,
  const Matrix<F>& d, Matrix<F>& X,
  bool checkIfSingular )
{
#ifndef RELEASE
    clique::PushCallStack("numeric::LocalFrontLDLDiagonalSolve");
    if( d.Width() != 1 )
        throw std::logic_error("d must be a column vector");
    if( d.Height() != X.Height() )
        throw std::logic_error("Invalid height of X");
#endif
    basic::DiagonalSolve( LEFT, NORMAL, d, X, checkIfSingular );
#ifndef RELEASE
    clique::PopCallStack();
#endif
}

template<typename F>
void clique::numeric::LocalFrontLDLForwardSolve
( int supernodeSize, const Matrix<F>& L, Matrix<F>& X )
{
#ifndef RELEASE
    clique::PushCallStack("numeric::LocalFrontLDLForwardSolve");
    if( L.Height() != L.Width() || L.Height() != X.Height() || 
        L.Height() < supernodeSize )
    {
        std::ostringstream msg;
        msg << "Nonconformal solve:\n"
            << "  supernodeSize ~ " << supernodeSize << "\n"
            << "  L ~ " << L.Height() << " x " << L.Width() << "\n"
            << "  X ~ " << X.Height() << " x " << X.Width() << "\n";
        throw std::logic_error( msg.str().c_str() );
    }
#endif
    Matrix<F> LT, LB;
    Matrix<F> XT, XB;

    LockedPartitionDown
    ( L, LT,
         LB, supernodeSize );
    PartitionDown
    ( X, XT,
         XB, supernodeSize );

    basic::Trsm( LEFT, LOWER, NORMAL, UNIT, (F)1, LT, XT );
    basic::Gemm( NORMAL, NORMAL, (F)-1, LB, XT, (F)1, XB );
#ifndef RELEASE
    clique::PopCallStack();
#endif
}

// The unit upper triangle and its (conjugate-)transpose 
// (with the exception of the diagonal) must be explicitly stored
template<typename F>
void clique::numeric::LocalFrontLDLBackwardSolve
( Orientation orientation, int supernodeSize, const Matrix<F>& L, Matrix<F>& X )
{
#ifndef RELEASE
    clique::PushCallStack("numeric::LocalFrontLDLBackwardSolve");
    if( L.Height() != L.Width() || L.Height() != X.Height() || 
        L.Height() < supernodeSize )
    {
        std::ostringstream msg;
        msg << "Nonconformal solve:\n"
            << "  supernodeSize ~ " << supernodeSize << "\n"
            << "  L ~ " << L.Height() << " x " << L.Width() << "\n"
            << "  X ~ " << X.Height() << " x " << X.Width() << "\n";
        throw std::logic_error( msg.str().c_str() );
    }
    if( orientation == NORMAL )
        throw std::logic_error("LDL must be (conjugate-)transposed");
#endif
    Matrix<F> LT, LB;
    Matrix<F> XT, XB;
    LockedPartitionDown
    ( L, LT, 
         LB, supernodeSize );
    PartitionDown
    ( X, XT,
         XB, supernodeSize );

    basic::Gemm( orientation, NORMAL, (F)-1, LB, XB, (F)1, XT );
    basic::Trsm( LEFT, LOWER, orientation, UNIT, (F)1, LT, XT );
#ifndef RELEASE
    clique::PopCallStack();
#endif
}

template void clique::numeric::LocalFrontLDLForwardSolve
( int supernodeSize,
  const Matrix<float>& L, Matrix<float>& X );
template void clique::numeric::LocalFrontLDLDiagonalSolve
( int supernodeSize,
  const Matrix<float>& d, Matrix<float>& X,
  bool checkIfSingular );
template void clique::numeric::LocalFrontLDLBackwardSolve
( Orientation orientation, int supernodeSize,
  const Matrix<float>& L, Matrix<float>& X );

template void clique::numeric::LocalFrontLDLForwardSolve
( int supernodeSize,
  const Matrix<double>& L, Matrix<double>& X );
template void clique::numeric::LocalFrontLDLDiagonalSolve
( int supernodeSize,
  const Matrix<double>& d, Matrix<double>& X,
  bool checkIfSingular );
template void clique::numeric::LocalFrontLDLBackwardSolve
( Orientation orientation, int supernodeSize,
  const Matrix<double>& L, Matrix<double>& X );

template void clique::numeric::LocalFrontLDLForwardSolve
( int supernodeSize,
  const Matrix<std::complex<float> >& L, Matrix<std::complex<float> >& X );
template void clique::numeric::LocalFrontLDLDiagonalSolve
( int supernodeSize,
  const Matrix<std::complex<float> >& d, Matrix<std::complex<float> >& X,
  bool checkIfSingular );
template void clique::numeric::LocalFrontLDLBackwardSolve
( Orientation orientation, int supernodeSize,
  const Matrix<std::complex<float> >& L, Matrix<std::complex<float> >& X );

template void clique::numeric::LocalFrontLDLForwardSolve
( int supernodeSize,
  const Matrix<std::complex<double> >& L, Matrix<std::complex<double> >& X );
template void clique::numeric::LocalFrontLDLDiagonalSolve
( int supernodeSize,
  const Matrix<std::complex<double> >& d, Matrix<std::complex<double> >& X,
  bool checkIfSingular );
template void clique::numeric::LocalFrontLDLBackwardSolve
( Orientation orientation, int supernodeSize,
  const Matrix<std::complex<double> >& L, Matrix<std::complex<double> >& X );