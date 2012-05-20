/*
   Modification of include/elemental/advanced/LDL.hpp from Elemental.
   Copyright (c) 2009-2012, Jack Poulson
   All rights reserved.

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
#ifndef CLIQUE_NUMERIC_LOCAL_FRONT_BLOCK_LDL_HPP
#define CLIQUE_NUMERIC_LOCAL_FRONT_BLOCK_LDL_HPP 1

namespace cliq {
namespace numeric {

template<typename F> 
void LocalFrontBlockLDL
( Orientation orientation, Matrix<F>& AL, Matrix<F>& ABR );

//----------------------------------------------------------------------------//
// Implementation begins here                                                 //
//----------------------------------------------------------------------------//

template<typename F> 
inline void LocalFrontBlockLDL
( Orientation orientation, Matrix<F>& AL, Matrix<F>& ABR )
{
#ifndef RELEASE
    PushCallStack("numeric::LocalFrontBlockLDL");
#endif
    Matrix<F> ATL,
              ABL;
    elem::PartitionDown
    ( AL, ATL,
          ABL, AL.Width() );
    
    // Make a copy of the original contents of ABL
    Matrix<F> BBL( ABL );

    // Call the standard routine
    LocalFrontLDL( orientation, AL, ABR );

    // Copy the original contents of ABL back
    ABL = BBL;

    // Overwrite ATL with inv(L D L^[T/H]) = L^[-T/H] D^{-1} L^{-1}
    Matrix<F> LTL( ATL );
    elem::TriangularInverse( LOWER, UNIT, ATL );
    elem::MakeTrapezoidal( LEFT, LOWER, 0, ATL );
    Matrix<F> d;
    ATL.GetDiagonal( d );
    Matrix<F> ones;
    Ones( d.Height(), d.Width(), ones );
    ATL.SetDiagonal( ones );
    elem::DiagonalSolve( LEFT, NORMAL, d, ATL );
    elem::Trsm( LEFT, LOWER, orientation, UNIT, (F)1, LTL, ATL );
#ifndef RELEASE
    PopCallStack();
#endif
}

} // namespace numeric
} // namespace cliq

#endif // CLIQUE_NUMERIC_LOCAL_FRONT_BLOCK_LDL_HPP
