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
#ifndef CLIQ_CORE_MULTIVEC_DECL_HPP
#define CLIQ_CORE_MULTIVEC_DECL_HPP

namespace cliq {

template<typename T>
class MultiVec
{
public:
    // Constructors and destructors
    MultiVec();
    MultiVec( int height, int width );
    // TODO: Constructor for building from a MultiVec
    ~MultiVec();

    // High-level information
    int Height() const;
    int Width() const;

    // Data
    T Get( int row, int col ) const;
    void Set( int row, int col, T value );
    void Update( int row, int col, T value );

    // For modifying the size of the multi-vector
    void Empty();
    void Resize( int height, int width );

    // Assignment
    const MultiVec<T>& operator=( const MultiVec<T>& X );

private:
    Matrix<T> multiVec_;
};

// Set all of the entries of X to zero
template<typename T>
void MakeZeros( MultiVec<T>& X );

// Draw the entries of X uniformly from the unitball in T
template<typename T>
void MakeUniform( MultiVec<T>& X );

// Just column-wise l2 norms for now
template<typename F>
void Norms( const MultiVec<F>& X, std::vector<BASE(F)>& norms );

// Just column-wise l2 norms for now
template<typename F>
BASE(F) Norm( const MultiVec<F>& x );

// Y := alpha X + Y 
template<typename T>
void Axpy( T alpha, const MultiVec<T>& X, MultiVec<T>& Y );

} // namespace cliq

#endif // ifndef CLIQ_CORE_MULTIVEC_DECL_HPP
