/*
   Copyright (c) 2009-2014, Jack Poulson, Lexing Ying,
   The University of Texas at Austin, Stanford University, and the
   Georgia Insitute of Technology.
   All rights reserved.
 
   This file is part of Clique and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CLIQ_NUMERIC_LOWERMULTIPLY_HPP
#define CLIQ_NUMERIC_LOWERMULTIPLY_HPP

namespace cliq {

template<typename T>
void LowerMultiply
( Orientation orientation, int diagOffset,
  const DistSymmInfo& info, const DistSymmFrontTree<T>& L, 
  DistNodalMultiVec<T>& X );

} // namespace cliq

//----------------------------------------------------------------------------//
// Implementation begins here                                                 //
//----------------------------------------------------------------------------//

#include "./lower_multiply/local_front.hpp"
#include "./lower_multiply/dist_front.hpp"

#include "./lower_multiply/local.hpp"
#include "./lower_multiply/dist.hpp"

namespace cliq {

template<typename T>
inline void LowerMultiply
( Orientation orientation, int diagOffset,
  const DistSymmInfo& info, const DistSymmFrontTree<T>& L, 
  DistNodalMultiVec<T>& X )
{
    DEBUG_ONLY(CallStackEntry cse("LowerMultiply"))
    if( orientation == NORMAL )
    {
        LocalLowerMultiplyNormal( diagOffset, info, L, X );
        DistLowerMultiplyNormal( diagOffset, info, L, X );
    }
    else
    {
        const bool conjugate = ( orientation==ADJOINT );
        DistLowerMultiplyTranspose( diagOffset, info, L, X, conjugate );
        LocalLowerMultiplyTranspose( diagOffset, info, L, X, conjugate );
    }
}

} // namespace cliq

#endif // ifndef CLIQ_NUMERIC_LOWERMULTIPLY_HPP
