/*
   Clique: a scalable implementation of the multifrontal algorithm

   Copyright (C) 2010-2011 Jack Poulson <jack.poulson@gmail.com>
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
#ifndef CLIQUE_NUMERIC_LDL_SOLVE_HPP
#define CLIQUE_NUMERIC_LDL_SOLVE_HPP 1

namespace clique {
namespace numeric {

template<typename F>
void LDLSolve
( Orientation orientation,
  const symbolic::LocalSymmFact& localS,
  const symbolic::DistSymmFact& distS,
  const numeric::LocalSymmFact<F>& localL,
  const numeric::DistSymmFact<F>& distL,
        Matrix<F>& localX, 
        bool checkIfSingular=true );

template<typename F>
void LocalLDLForwardSolve
( const symbolic::LocalSymmFact& S, 
  const numeric::LocalSymmFact<F>& L, Matrix<F>& X );

template<typename F>
void DistLDLForwardSolve
( const symbolic::DistSymmFact& S,
  const numeric::LocalSymmFact<F>& localL,
  const numeric::DistSymmFact<F>& distL,
        Matrix<F>& localX );

template<typename F>
void LocalLDLDiagonalSolve
( const symbolic::LocalSymmFact& S,
  const numeric::LocalSymmFact<F>& L, 
        Matrix<F>& X,
        bool checkIfSingular=false );

template<typename F>
void DistLDLDiagonalSolve
( const symbolic::DistSymmFact& S,
  const numeric::DistSymmFact<F>& L,
        Matrix<F>& localX,
        bool checkIfSingular=true );

template<typename F>
void LocalLDLBackwardSolve
( Orientation orientation,
  const symbolic::LocalSymmFact& S, 
  const numeric::LocalSymmFact<F>& localL, 
  const numeric::DistSymmFact<F>& distL,
        Matrix<F>& X );

template<typename F>
void DistLDLBackwardSolve
( Orientation orientation,
  const symbolic::DistSymmFact& S,
  const numeric::DistSymmFact<F>& L,
        Matrix<F>& localX );

//----------------------------------------------------------------------------//
// Implementation begins here                                                 //
//----------------------------------------------------------------------------//

template<typename F>
void LDLSolve
( Orientation orientation,
  const symbolic::LocalSymmFact& localS,
  const symbolic::DistSymmFact& distS,
  const numeric::LocalSymmFact<F>& localL,
  const numeric::DistSymmFact<F>& distL,
        Matrix<F>& localX, 
        bool checkIfSingular )
{
#ifndef RELEASE
    PushCallStack("numeric::LDLSolve");
#endif
    clique::numeric::LocalLDLForwardSolve( localS, localL, localX );
    clique::numeric::DistLDLForwardSolve( distS, localL, distL, localX );
    clique::numeric::LocalLDLDiagonalSolve( localS, localL, localX, true );
    clique::numeric::DistLDLDiagonalSolve( distS, distL, localX, true );
    clique::numeric::DistLDLBackwardSolve( ADJOINT, distS, distL, localX );
    clique::numeric::LocalLDLBackwardSolve
    ( ADJOINT, localS, localL, distL, localX );
#ifndef RELEASE
    PopCallStack();
#endif
}

} // namespace numeric
} // namespace clique

#endif /* CLIQUE_NUMERIC_LDL_SOLVE_HPP */

