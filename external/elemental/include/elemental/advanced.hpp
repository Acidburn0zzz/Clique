/*
   Copyright (c) 2009-2011, Jack Poulson
   All rights reserved.

   This file is part of Elemental.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

    - Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    - Neither the name of the owner nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef ELEMENTAL_ADVANCED_HPP
#define ELEMENTAL_ADVANCED_HPP 1

#include "elemental/basic.hpp"

namespace elemental {
namespace advanced {

//----------------------------------------------------------------------------//
// Cholesky:                                                                  //
//                                                                            //
// Overwrite a triangle of A with the Cholesky factor of A. 'shape'           //
// determines whether it is the upper or lower triangle.                      //
//----------------------------------------------------------------------------//

// Serial version
template<typename F>
void Cholesky( Shape shape, Matrix<F>& A );

// Parallel version
template<typename F>
void Cholesky( Shape shape, DistMatrix<F,MC,MR>& A );

//----------------------------------------------------------------------------//
// CholeskySolve:                                                             //
//                                                                            //
// Overwrites X := inv(A) X, where A is Hermitian positive-definite, using a  //
// Cholesky factorization of A.                                               //
//----------------------------------------------------------------------------//

//TODO: Serial version

// Parallel version
template<typename F>
void CholeskySolve
( Shape shape, DistMatrix<F,MC,MR>& A, DistMatrix<F,MC,MR>& X );

//----------------------------------------------------------------------------//
// GaussianElimination:                                                       //
//                                                                            //
// Uses an LU factorization with partial pivoting to overwrite B := A^-1 B    //
//----------------------------------------------------------------------------//

// TODO: Add a serial version

// Parallel version
template<typename F>
void GaussianElimination( DistMatrix<F,MC,MR>& A, DistMatrix<F,MC,MR>& B );

//----------------------------------------------------------------------------//
// HermitianGenDefiniteEig (Hermitian Generalized-Definite Eigensolver)       //
//                                                                            //
//----------------------------------------------------------------------------//

#ifndef WITHOUT_PMRRR
// Grab the full set of eigenpairs of real symmetric A and SPD B
template<typename R>
void HermitianGenDefiniteEig
( HermitianGenDefiniteEigType type, Shape shape, 
  DistMatrix<R,MC,  MR>& A, 
  DistMatrix<R,MC,  MR>& B, 
  DistMatrix<R,STAR,VR>& w,
  DistMatrix<R,MC,  MR>& X,
  bool tryForHighAccuracy = false );
// Grab a partial set of eigenpairs. 
// The partial set is determined by the inclusive zero-indexed range 
//   a,a+1,...,b    ; a >= 0, b < n  
// of the n eigenpairs sorted from smallest to largest eigenvalues.  
template<typename R>
void HermitianGenDefiniteEig
( HermitianGenDefiniteEigType type, Shape shape,
  DistMatrix<R,MC,  MR>& A,
  DistMatrix<R,MC,  MR>& B,
  DistMatrix<R,STAR,VR>& w,
  DistMatrix<R,MC,  MR>& X,
  int a, int b, bool tryForHighAccuracy = false );
// Grab a partial set of eigenpairs.
// The partial set is determined by the half-open interval (a,b]
template<typename R>
void HermitianGenDefiniteEig
( HermitianGenDefiniteEigType type, Shape shape,
  DistMatrix<R,MC,  MR>& A,
  DistMatrix<R,MC,  MR>& B,
  DistMatrix<R,STAR,VR>& w,
  DistMatrix<R,MC,  MR>& X,
  R a, R b, bool tryForHighAccuracy = false );
// Grab the full set of eigenvalues of real symmetric A and SPD B
template<typename R>
void HermitianGenDefiniteEig
( HermitianGenDefiniteEigType type, Shape shape, 
  DistMatrix<R,MC,  MR>& A, 
  DistMatrix<R,MC,  MR>& B, 
  DistMatrix<R,STAR,VR>& w,
  bool tryForHighAccuracy = false );
// Grab a partial set of eigenvalues. 
// The partial set is determined by the inclusive zero-indexed range 
//   a,a+1,...,b    ; a >= 0, b < n  
// of the n eigenpairs sorted from smallest to largest eigenvalues.  
template<typename R>
void HermitianGenDefiniteEig
( HermitianGenDefiniteEigType type, Shape shape,
  DistMatrix<R,MC,  MR>& A,
  DistMatrix<R,MC,  MR>& B,
  DistMatrix<R,STAR,VR>& w,
  int a, int b, bool tryForHighAccuracy = false );
// Grab a partial set of eigenvalues.
// The partial set is determined by the half-open interval (a,b]
template<typename R>
void HermitianGenDefiniteEig
( HermitianGenDefiniteEigType type, Shape shape,
  DistMatrix<R,MC,  MR>& A,
  DistMatrix<R,MC,  MR>& B,
  DistMatrix<R,STAR,VR>& w,
  R a, R b, bool tryForHighAccuracy = false );

#ifndef WITHOUT_COMPLEX
// Grab the full set of eigenpairs of complex Hermitian A and HPD B
template<typename R>
void HermitianGenDefiniteEig    
( HermitianGenDefiniteEigType type, Shape shape,
  DistMatrix<std::complex<R>,MC,  MR>& A,
  DistMatrix<std::complex<R>,MC,  MR>& B,
  DistMatrix<             R, STAR,VR>& w,
  DistMatrix<std::complex<R>,MC,  MR>& X,
  bool tryForHighAccuracy = false );
// Grab a partial set of eigenpairs. 
// The partial set is determined by the inclusive zero-indexed range 
//   a,a+1,...,b    ; a >= 0, b < n  
// of the n eigenpairs sorted from smallest to largest eigenvalues.  
template<typename R>
void HermitianGenDefiniteEig
( HermitianGenDefiniteEigType type, Shape shape,
  DistMatrix<std::complex<R>,MC,  MR>& A,
  DistMatrix<std::complex<R>,MC,  MR>& B,
  DistMatrix<             R, STAR,VR>& w,
  DistMatrix<std::complex<R>,MC,  MR>& X,
  int a, int b, bool tryForHighAccuracy = false );
// Grab a partial set of eigenpairs.
// The partial set is determined by the half-open interval (a,b]
template<typename R>
void HermitianGenDefiniteEig
( HermitianGenDefiniteEigType type, Shape shape,
  DistMatrix<std::complex<R>,MC,  MR>& A,
  DistMatrix<std::complex<R>,MC,  MR>& B,
  DistMatrix<             R, STAR,VR>& w,
  DistMatrix<std::complex<R>,MC,  MR>& X,
  R a, R b, bool tryForHighAccuracy = false );
// Grab the full set of eigenvalues of complex Hermitian A and HPD B
template<typename R>
void HermitianGenDefiniteEig    
( HermitianGenDefiniteEigType type, Shape shape,
  DistMatrix<std::complex<R>,MC,  MR>& A,
  DistMatrix<std::complex<R>,MC,  MR>& B,
  DistMatrix<             R, STAR,VR>& w,
  bool tryForHighAccuracy = false );
// Grab a partial set of eigenvalues. 
// The partial set is determined by the inclusive zero-indexed range 
//   a,a+1,...,b    ; a >= 0, b < n  
// of the n eigenpairs sorted from smallest to largest eigenvalues.  
template<typename R>
void HermitianGenDefiniteEig
( HermitianGenDefiniteEigType type, Shape shape,
  DistMatrix<std::complex<R>,MC,  MR>& A,
  DistMatrix<std::complex<R>,MC,  MR>& B,
  DistMatrix<             R, STAR,VR>& w,
  int a, int b, bool tryForHighAccuracy = false );
// Grab a partial set of eigenvalues.
// The partial set is determined by the half-open interval (a,b]
template<typename R>
void HermitianGenDefiniteEig
( HermitianGenDefiniteEigType type, Shape shape,
  DistMatrix<std::complex<R>,MC,  MR>& A,
  DistMatrix<std::complex<R>,MC,  MR>& B,
  DistMatrix<             R, STAR,VR>& w,
  R a, R b, bool tryForHighAccuracy = false );
#endif // WITHOUT_COMPLEX
#endif // WITHOUT_PMRRR

//----------------------------------------------------------------------------//
// Hegst (HErmitian GEneralized to STandard eigenvalue problem):              //
//                                                                            //
// If side==LEFT,                                                             //
//   reduce the problems                                                      //
//                      A B X = X Lambda to A X = X Lambda                    //
//                      B A X = X Lambda to A X = X Lambda                    //
// If side==RIGHT,                                                            //
//   reduce the problem A X = B X Lambda to A X = X Lambda                    //
//                                                                            //
// D contains the Cholesky factor of B in the triangle corresponding to the   //
// parameter 'shape'.                                                         //
//----------------------------------------------------------------------------//

// Serial version
template<typename F>
void Hegst
( Side side, Shape shape, 
  Matrix<F>& A, const Matrix<F>& B );

// Parallel version
template<typename F>
void Hegst
( Side side, Shape shape, 
  DistMatrix<F,MC,MR>& A, const DistMatrix<F,MC,MR>& B );

//----------------------------------------------------------------------------//
// HermitianEig (Hermitian Eigensolver)                                       //
//                                                                            //
//----------------------------------------------------------------------------//

#ifndef WITHOUT_PMRRR
// Grab the full set of eigenpairs of the real, symmetric matrix A
void HermitianEig
( Shape shape, 
  DistMatrix<double,MC,  MR>& A, 
  DistMatrix<double,STAR,VR>& w,
  DistMatrix<double,MC,  MR>& Z,
  bool tryForHighAccuracy = false );
// Grab a partial set of eigenpairs of the real, symmetric n x n matrix A. 
// The partial set is determined by the inclusive zero-indexed range 
//   a,a+1,...,b    ; a >= 0, b < n  
// of the n eigenpairs sorted from smallest to largest eigenvalues.  
void HermitianEig
( Shape shape,
  DistMatrix<double,MC,  MR>& A,
  DistMatrix<double,STAR,VR>& w,
  DistMatrix<double,MC,  MR>& Z,
  int a, int b, bool tryForHighAccuracy = false );
// Grab a partial set of eigenpairs of the real, symmetric n x n matrix A. 
// The partial set is determined by the half-open interval (a,b]
void HermitianEig
( Shape shape,
  DistMatrix<double,MC,  MR>& A,
  DistMatrix<double,STAR,VR>& w,
  DistMatrix<double,MC,  MR>& Z,
  double a, double b, bool tryForHighAccuracy = false );
// Grab the full set of eigenvalues of the real, symmetric matrix A
void HermitianEig
( Shape shape,
  DistMatrix<double,MC,  MR>& A,
  DistMatrix<double,STAR,VR>& w,
  bool tryForHighAccuracy = false );
// Grab a partial set of eigenvalues of the real, symmetric n x n matrix A. 
// The partial set is determined by the inclusive zero-indexed range 
//   a,a+1,...,b    ; a >= 0, b < n  
// of the n eigenpairs sorted from smallest to largest eigenvalues.  
void HermitianEig
( Shape shape,
  DistMatrix<double,MC,  MR>& A,
  DistMatrix<double,STAR,VR>& w,
  int a, int b, bool tryForHighAccuracy = false );
// Grab a partial set of eigenvalues of the real, symmetric n x n matrix A. 
// The partial set is determined by the half-open interval (a,b]
void HermitianEig
( Shape shape,
  DistMatrix<double,MC,  MR>& A,
  DistMatrix<double,STAR,VR>& w,
  double a, double b, bool tryForHighAccuracy = false );
#ifndef WITHOUT_COMPLEX
// Grab the full set of eigenpairs of the complex, Hermitian matrix A
void HermitianEig    
( Shape shape,
  DistMatrix<std::complex<double>,MC,  MR>& A,
  DistMatrix<             double, STAR,VR>& w,
  DistMatrix<std::complex<double>,MC,  MR>& Z,
  bool tryForHighAccuracy = false );
// Grab a partial set of eigenpairs of the complex, Hermitian n x n matrix A. 
// The partial set is determined by the inclusive zero-indexed range 
//   a,a+1,...,b    ; a >= 0, b < n  
// of the n eigenpairs sorted from smallest to largest eigenvalues.  
void HermitianEig
( Shape shape,
  DistMatrix<std::complex<double>,MC,  MR>& A,
  DistMatrix<             double, STAR,VR>& w,
  DistMatrix<std::complex<double>,MC,  MR>& Z,
  int a, int b, bool tryForHighAccuracy = false );
// Grab a partial set of eigenpairs of the complex, Hermitian n x n matrix A. 
// The partial set is determined by the half-open interval (a,b]
void HermitianEig
( Shape shape,
  DistMatrix<std::complex<double>,MC,  MR>& A,
  DistMatrix<             double, STAR,VR>& w,
  DistMatrix<std::complex<double>,MC,  MR>& Z,
  double a, double b, bool tryForHighAccuracy = false );
// Grab the full set of eigenvalues of the complex, Hermitian matrix A
void HermitianEig
( Shape shape,
  DistMatrix<std::complex<double>,MC,  MR>& A,
  DistMatrix<             double, STAR,VR>& w,
  bool tryForHighAccuracy = false );
// Grab a partial set of eigenvalues of the complex, Hermitian n x n matrix A. 
// The partial set is determined by the inclusive zero-indexed range 
//   a,a+1,...,b    ; a >= 0, b < n  
// of the n eigenpairs sorted from smallest to largest eigenvalues.  
void HermitianEig
( Shape shape,
  DistMatrix<std::complex<double>,MC,  MR>& A,
  DistMatrix<             double, STAR,VR>& w,
  int a, int b, bool tryForHighAccuracy = false );
// Grab a partial set of eigenvalues of the complex, Hermitian n x n matrix A. 
// The partial set is determined by the half-open interval (a,b]
void HermitianEig
( Shape shape,
  DistMatrix<std::complex<double>,MC,  MR>& A,
  DistMatrix<             double, STAR,VR>& w,
  double a, double b, bool tryForHighAccuracy = false );
#endif // WITHOUT_COMPLEX
#endif // WITHOUT_PMRRR

//----------------------------------------------------------------------------//
// HouseholderSolve:                                                          //
//                                                                            //
// Overwrite X with the solution of inv(A) X or inv(A)^H X, where A need not  //
// be square. NOTE: If the system is underdetermined, then X should be the    //
// size of the solution and the input RHS should be stored in the top m rows  //
// of X, if the underdetermined matrix is m x n.                              //
//----------------------------------------------------------------------------//

//TODO: Write the serial version

// Parallel versions
template<typename R>
void HouseholderSolve
( Orientation orientation, DistMatrix<R,MC,MR>& A, DistMatrix<R,MC,MR>& X );
template<typename R>
void HouseholderSolve
( Orientation orientation, 
  DistMatrix<std::complex<R>,MC,MR>& A,
  DistMatrix<std::complex<R>,MC,MR>& X );

//----------------------------------------------------------------------------//
// LDLH (LDL^H factorization):                                                //
//                                                                            //
// Overwrite the lower triangle of A with L and d with the diagonal entries   //
// of D, so that A = L D L^H.                                                 //
//                                                                            //
// Partial pivoting is not yet supported.                                     //
//----------------------------------------------------------------------------//

// Serial version (currently unblocked)
template<typename F>
void LDLH( Matrix<F>& A, Matrix<F>& d );

// Parallel version
template<typename F>
void LDLH( DistMatrix<F,MC,MR>& A, DistMatrix<F,MC,STAR>& d );

//----------------------------------------------------------------------------//
// LDLT (LDL^T factorization):                                                //
//                                                                            //
// Overwrite the lower triangle of A with L and d with the diagonal entries   //
// of D, so that A = L D L^T.                                                 //
//                                                                            //
// Partial pivoting is not yet supported.                                     //
//----------------------------------------------------------------------------//

// Serial version (currently unblocked)
template<typename F>
void LDLT( Matrix<F>& A, Matrix<F>& d );

// Parallel version
template<typename F>
void LDLT( DistMatrix<F,MC,MR>& A, DistMatrix<F,MC,STAR>& d );

//----------------------------------------------------------------------------//
// LU (LU factorization):                                                     //
//                                                                            //
// If a container for a pivot vector is passed in, then A is overwritten with //
// its LU factorization after partial pivoting: P A = L U.                    //
// P is compressed into the vector p by storing the location of the nonzero   //
// element of each row.                                                       //
//                                                                            //
// If pivot vector is given, then A is overwritten with L U. Note that this   //
// version should usually be avoided, as pivoting is usually required for     //
// stability.                                                                 //
//----------------------------------------------------------------------------//

template<typename F>
void LU( Matrix<F>& A );

template<typename F> 
void LU( Matrix<F>& A, Matrix<int>& p );

template<typename F>
void LU( DistMatrix<F,MC,MR>& A );

template<typename F>
void LU( DistMatrix<F,MC,MR>& A, DistMatrix<int,VC,STAR>& p );

//----------------------------------------------------------------------------//
// LQ (LQ factorization):                                                     //
//                                                                            //
// Essentially the adjoint of a QR factorization on the adjoint of the input  //
// matrix.                                                                    //
//----------------------------------------------------------------------------//

// Serial version for real datatypes
template<typename R>
void LQ( Matrix<R>& A );

#ifndef WITHOUT_COMPLEX
// Serial version for complex datatypes
template<typename R>
void LQ( Matrix<std::complex<R> >& A, Matrix<std::complex<R> >& t );
#endif

// Parallel version for real datatypes
template<typename R>
void LQ( DistMatrix<R,MC,MR>& A );

#ifndef WITHOUT_COMPLEX
// Parallel version for complex datatypes
template<typename R>
void LQ
( DistMatrix<std::complex<R>,MC,MR  >& A, 
  DistMatrix<std::complex<R>,MD,STAR>& t );
#endif


//----------------------------------------------------------------------------//
// Norm                                                                       //
//----------------------------------------------------------------------------//

template<typename R>
R Norm( const Matrix<R>& A, NormType type=FROBENIUS_NORM );

template<typename R>
R Norm( const DistMatrix<R,MC,MR>& A, NormType type=FROBENIUS_NORM );

#ifndef WITHOUT_COMPLEX
template<typename R>
R Norm( const Matrix<std::complex<R> >& A, NormType type=FROBENIUS_NORM );

template<typename R>
R Norm
( const DistMatrix<std::complex<R>,MC,MR>& A, NormType type=FROBENIUS_NORM );
#endif

//----------------------------------------------------------------------------//
// HermitianNorm                                                              //
//----------------------------------------------------------------------------//

template<typename R>
R HermitianNorm
( Shape shape, const Matrix<R>& A, 
  NormType type=FROBENIUS_NORM );

template<typename R>
R HermitianNorm
( Shape shape, const DistMatrix<R,MC,MR>& A, 
  NormType type=FROBENIUS_NORM );

#ifndef WITHOUT_COMPLEX
template<typename R>
R HermitianNorm
( Shape shape, const Matrix<std::complex<R> >& A, 
  NormType type=FROBENIUS_NORM );

template<typename R>
R HermitianNorm
( Shape shape, const DistMatrix<std::complex<R>,MC,MR>& A, 
  NormType type=FROBENIUS_NORM );
#endif

//----------------------------------------------------------------------------//
// SymmetricNorm                                                              //
//----------------------------------------------------------------------------//

template<typename R>
R SymmetricNorm
( Shape shape, const Matrix<R>& A, 
  NormType type=FROBENIUS_NORM );

template<typename R>
R SymmetricNorm
( Shape shape, const DistMatrix<R,MC,MR>& A, 
  NormType type=FROBENIUS_NORM );

#ifndef WITHOUT_COMPLEX
template<typename R>
R SymmetricNorm
( Shape shape, const Matrix<std::complex<R> >& A, 
  NormType type=FROBENIUS_NORM );

template<typename R>
R SymmetricNorm
( Shape shape, const DistMatrix<std::complex<R>,MC,MR>& A, 
  NormType type=FROBENIUS_NORM );
#endif

//----------------------------------------------------------------------------//
// QR (QR factorization):                                                     //
//                                                                            //
// Performs a Householder QR factorization that overwrites the upper triangle //
// of A with R and fills the lower triangle with the scaled Householder       //
// transforms used to generate Q (they are implicitly one on the diagonal of  //
// A). The scaling factors for the Householder transforms are stored in t.    //
//                                                                            //
// For the complex case, 't' holds the Householder reflection coefficients    //
// that define the Householder transformation                                 //
//     House(tau,u) = I - tau u u^H                                           //
//                                                                            //
// IMPORTANT NOTE: The LAPACK convention for early-exiting when computing the //
// Householder reflection for a vector a = [ alpha11, a12 ]^T, where          //
// || a12 ||_2 = 0 and Im( alpha11 ) = 0, is to set 'tau' to zero in the      //
// Householder reflector equation:                                            //
//                                                                            //
//   House(tau,u) = I - tau u u^H                                             //
//                                                                            //
// which is not a valid Householder reflection due to the requirement that    //
// u be normalizable. We thus take the approach of setting tau = 2 when       //
// || a12 ||_2 = 0 and Im( alpha11 ) = 0, so that                             //
//                                                                            //
//   House(2,u) a = (I - 2 | 1 | | 1 0 | ) | alpha11 | = | -alpha11 |         //
//                         | 0 |           |    0    |   |     0    |         //
//                                                                            //
// This allows for the computation of the triangular matrix in the Compact WY //
// transform / UT transform to be computed mainly with Level 3 BLAS.          //
//----------------------------------------------------------------------------//

// Serial version for real datatypes
template<typename R>
void QR( Matrix<R>& A );

#ifndef WITHOUT_COMPLEX
// Serial version for complex datatypes
template<typename R>
void QR( Matrix<std::complex<R> >& A, Matrix<std::complex<R> >& t );
#endif

// Parallel version for real datatypes
template<typename R>
void QR( DistMatrix<R,MC,MR>& A );

#ifndef WITHOUT_COMPLEX
// Parallel version for complex datatypes
template<typename R>
void QR
( DistMatrix<std::complex<R>,MC,MR  >& A, 
  DistMatrix<std::complex<R>,MD,STAR>& t );
#endif

//----------------------------------------------------------------------------//
// Reflector (Householder reflector):                                         //
//----------------------------------------------------------------------------//

// Serial version for real datatypes
template<typename R>
R
Reflector( Matrix<R>& chi, Matrix<R>& x );

#ifndef WITHOUT_COMPLEX
// Serial version for complex datatypes
template<typename R>
std::complex<R>
Reflector( Matrix<std::complex<R> >& chi, Matrix<std::complex<R> >& x );
#endif

// Parallel version
template<typename F>
F
Reflector( DistMatrix<F,MC,MR>& chi, DistMatrix<F,MC,MR>& x );

//----------------------------------------------------------------------------//
// SkewHermitianEig (Skew-Hermitian Eigensolver)                              //
//                                                                            //
//----------------------------------------------------------------------------//

#ifndef WITHOUT_COMPLEX
#ifndef WITHOUT_PMRRR
// Grab the full set of eigenpairs of the real, skew-symmetric matrix G
void SkewHermitianEig
( Shape shape, 
  DistMatrix<double,              MC,  MR>& G, 
  DistMatrix<std::complex<double>,STAR,VR>& w,
  DistMatrix<std::complex<double>,MC,  MR>& Z,
  bool tryForHighAccuracy = false );
// Grab a partial set of eigenpairs of the real, skew-symmetric n x n matrix G. 
// The partial set is determined by the inclusive zero-indexed range 
//   a,a+1,...,b    ; a >= 0, b < n  
// of the n eigenpairs sorted from smallest to largest eigenvalues.  
void SkewHermitianEig
( Shape shape,
  DistMatrix<double,              MC,  MR>& G,
  DistMatrix<std::complex<double>,STAR,VR>& w,
  DistMatrix<std::complex<double>,MC,  MR>& Z,
  int a, int b, bool tryForHighAccuracy = false );
// Grab a partial set of eigenpairs of the real, skew-symmetric n x n matrix G. 
// The partial set is determined by the half-open imaginary interval (a,b]
void SkewHermitianEig
( Shape shape,
  DistMatrix<double,              MC,  MR>& G,
  DistMatrix<std::complex<double>,STAR,VR>& w,
  DistMatrix<std::complex<double>,MC,  MR>& Z,
  double a, double b, bool tryForHighAccuracy = false );
// Grab the full set of eigenvalues of the real, skew-symmetric matrix G 
void SkewHermitianEig
( Shape shape,
  DistMatrix<double,              MC,  MR>& G,
  DistMatrix<std::complex<double>,STAR,VR>& w,
  bool tryForHighAccuracy = false );
// Grab a partial set of eigenvalues of the real, skew-symmetric n x n matrix G.
// The partial set is determined by the inclusive zero-indexed range 
//   a,a+1,...,b    ; a >= 0, b < n  
// of the n eigenpairs sorted from smallest to largest eigenvalues.  
void SkewHermitianEig
( Shape shape,
  DistMatrix<double,              MC,  MR>& G,
  DistMatrix<std::complex<double>,STAR,VR>& w,
  int a, int b, bool tryForHighAccuracy = false );
// Grab a partial set of eigenvalues of the real, skew-symmetric n x n matrix G.
// The partial set is determined by the half-open imaginary interval (a,b]
void SkewHermitianEig
( Shape shape,
  DistMatrix<double,              MC,  MR>& G,
  DistMatrix<std::complex<double>,STAR,VR>& w,
  double a, double b, bool tryForHighAccuracy = false );

// Grab the full set of eigenpairs of the complex, skew-Hermitian matrix G 
void SkewHermitianEig    
( Shape shape,
  DistMatrix<std::complex<double>,MC,  MR>& G,
  DistMatrix<std::complex<double>,STAR,VR>& w,
  DistMatrix<std::complex<double>,MC,  MR>& Z,
  bool tryForHighAccuracy = false );
// Grab a partial set of eigenpairs of the complex, skew-Hermitian n x n matrix
// G. The partial set is determined by the inclusive zero-indexed range 
//   a,a+1,...,b    ; a >= 0, b < n  
// of the n eigenpairs sorted from smallest to largest eigenvalues.  
void SkewHermitianEig
( Shape shape,
  DistMatrix<std::complex<double>,MC,  MR>& G,
  DistMatrix<std::complex<double>,STAR,VR>& w,
  DistMatrix<std::complex<double>,MC,  MR>& Z,
  int a, int b, bool tryForHighAccuracy = false );
// Grab a partial set of eigenpairs of the complex, skew-Hermitian n x n matrix
// G. The partial set is determined by the half-open interval (a,b]
void SkewHermitianEig
( Shape shape,
  DistMatrix<std::complex<double>,MC,  MR>& G,
  DistMatrix<std::complex<double>,STAR,VR>& w,
  DistMatrix<std::complex<double>,MC,  MR>& Z,
  double a, double b, bool tryForHighAccuracy = false );
// Grab the full set of eigenvalues of the complex, skew-Hermitian matrix G 
void SkewHermitianEig
( Shape shape,
  DistMatrix<std::complex<double>,MC,  MR>& G,
  DistMatrix<std::complex<double>,STAR,VR>& w,
  bool tryForHighAccuracy = false );
// Grab a partial set of eigenvalues of the complex, skew-Hermitian n x n matrix
// G. The partial set is determined by the inclusive zero-indexed range 
//   a,a+1,...,b    ; a >= 0, b < n  
// of the n eigenpairs sorted from smallest to largest eigenvalues.  
void SkewHermitianEig
( Shape shape,
  DistMatrix<std::complex<double>,MC,  MR>& G,
  DistMatrix<std::complex<double>,STAR,VR>& w,
  int a, int b, bool tryForHighAccuracy = false );
// Grab a partial set of eigenvalues of the complex, skew-Hermitian n x n matrix
// G. The partial set is determined by the half-open imaginary interval (a,b]
void SkewHermitianEig
( Shape shape,
  DistMatrix<std::complex<double>,MC,  MR>& G,
  DistMatrix<std::complex<double>,STAR,VR>& w,
  double a, double b, bool tryForHighAccuracy = false );
#endif // WITHOUT_PMRRR
#endif // WITHOUT_COMPLEX

//----------------------------------------------------------------------------//
// SVD (Singular Value Decomposition):                                        //
//                                                                            //
// Two approaches:                                                            //
// (1)                                                                        //
//   Compute the skinny SVD of A, A = U Sigma V^T, where, if A is m x n,      //
//   U is m x min(m,n) and V^T is min(m,n) x n. We store U in A, V^T in V,    //
//   and Sigma in SigmaDiag.                                                  //
// (2)                                                                        //
//   Compute the skinny SVD of A, A = U Sigma V^T, where, if A is m x n,      //
//   U is m x min(m,n) and V^T is min(m,n) x n. We store U in U, V^T in V,    //
//   and Sigma in SigmaDiag.                                                  //
//----------------------------------------------------------------------------//

// Serial versions where A is overwritten by U
template<typename R>
void SVD( Matrix<R>& A, Matrix<R>& VT, std::vector<R>& SigmaDiag );

#ifndef WITHOUT_COMPLEX
template<typename R>
void SVD
( Matrix<std::complex<R> >& A, Matrix<std::complex<R> >& VT, 
  std::vector<R>& SigmaDiag );     
#endif

// Serial versions where A is not overwritten
template<typename R>
void
SVD( Matrix<R>& A, Matrix<R>& U, Matrix<R>& VT, std::vector<R>& SigmaDiag );

#ifndef WITHOUT_COMPLEX
template<typename R>
void
SVD
( Matrix<std::complex<R> >& A, Matrix<std::complex<R> >& U, 
  Matrix<std::complex<R> >& VT, std::vector<R>& SigmaDiag );
#endif

//----------------------------------------------------------------------------//
// HermitianTridiag (Reduce Hermitian matrix to tridiagonal form):            //
//                                                                            //
// The diagonal and sub/super-diagonal of A are overwritten with a similar    //
// tridiagonal matrix that is found by successively applying Householder      //
// reflections to zero the matrix outside of the tridiagonal band.            //
//                                                                            //
// 'shape' decided which triangle of A specifies the Hermitian matrix, and on //
// exit the transforms are stored above the super/sub-diagonal and are        //
// implicitly one on the super/sub-diagonal.                                  //
//                                                                            //
// See the above note for QR factorizations detailing 't' and the difference  //
// in Householder transform early-exit approaches for the serial and parallel //
// routines.                                                                  //
//----------------------------------------------------------------------------//

// Serial version for real datatypes (currently unblocked)
template<typename R>
void HermitianTridiag( Shape shape, Matrix<R>& A );

#ifndef WITHOUT_COMPLEX
// Serial version for complex datatypes (currently unblocked)
template<typename R>
void HermitianTridiag
( Shape shape, Matrix<std::complex<R> >& A, Matrix<std::complex<R> >& t );
#endif

// Parallel version for real datatypes
template<typename R>
void HermitianTridiag( Shape shape, DistMatrix<R,MC,MR>& A );

#ifndef WITHOUT_COMPLEX
// Parallel version for complex datatypes
template<typename R>
void HermitianTridiag
( Shape shape,
  DistMatrix<std::complex<R>,MC,  MR  >& A,
  DistMatrix<std::complex<R>,STAR,STAR>& t );
#endif

void SetHermitianTridiagApproach( HermitianTridiagApproach approach );
HermitianTridiagApproach GetHermitianTridiagApproach();

// If dropping down to a square grid, the two simplest approaches are to take 
// the first r^2 processes from the original grid (for an r x r grid) and to
// either order them column-major or row-major to form the square grid.
void SetHermitianTridiagGridOrder( GridOrder order );
GridOrder GetHermitianTridiagGridOrder();

//----------------------------------------------------------------------------//
// TriangularInverse                                                          //
//                                                                            //
// Inverts a triangular matrix. 'shape' determines whether A is assumed to be //
// upper or lower triangular, and 'diagonal' determines whether or not A is   //
// to be treated as having a unit diagonal.                                   //
//----------------------------------------------------------------------------//

// Serial version
template<typename F>
void
TriangularInverse
( Shape shape, Diagonal diagonal, Matrix<F>& A );

// Parallel version
template<typename F>
void
TriangularInverse
( Shape shape, Diagonal diagonal, DistMatrix<F,MC,MR>& A  );

//----------------------------------------------------------------------------//
// ApplyPackedReflectors                                                      //
//                                                                            //
// Applies the accumulated Householder transforms that are stored in the      //
// triangle of H specified by 'shape' to the matrix A.                        //
//                                                                            //
// If 'shape' is set to 'LOWER', then offset determines the diagonal that the //
// transforms are stored above (they are implicitly one on that diagonal).    //
//                                                                            //
// If 'shape' is set to 'UPPER', then offset determines the diagonal that the //
// transforms are stored below (they are implicitly one on that diagonal).    //
//                                                                            //
// 'direction' determines whether the reflectors are stored vertically or     //
// horizontally.                                                              //
//                                                                            //
// 'conjugation' determines whether or not the Householder scalars should be  //
// conjugated.                                                                //
//                                                                            //
// If 'order' is set to forward, then the reflectors are applied              //
// left-to-right, or top-to-bottom, depending on 'direction'. Otherwise, they //
// are applied in the opposite order.
//                                                                            //
// See the above note for QR factorizations regarding the vector 't' and      //
// Householder early-exit conditions.                                         //
//----------------------------------------------------------------------------//

// TODO: Add serial versions

// Parallel version for real datatypes
template<typename R>
void ApplyPackedReflectors
( Side side, Shape shape, VectorDirection direction, ForwardOrBackward order,
  int offset,
  const DistMatrix<R,MC,MR>& H, 
        DistMatrix<R,MC,MR>& A );

#ifndef WITHOUT_COMPLEX
template<typename R>
void ApplyPackedReflectors
( Side side, Shape shape,
  VectorDirection direction, ForwardOrBackward order, Conjugation conjugation,
  int offset,
  const DistMatrix<std::complex<R>,MC,  MR  >& H,
  const DistMatrix<std::complex<R>,MD,  STAR>& t,
        DistMatrix<std::complex<R>,MC,  MR  >& A );
template<typename R>
void ApplyPackedReflectors
( Side side, Shape shape, 
  VectorDirection direction, ForwardOrBackward order, Conjugation conjugation,
  int offset,
  const DistMatrix<std::complex<R>,MC,  MR  >& H,
  const DistMatrix<std::complex<R>,STAR,STAR>& t,
        DistMatrix<std::complex<R>,MC,  MR  >& A );
#endif

} // advanced
} // elemental

//----------------------------------------------------------------------------//
// Implementation begins here                                                 //
//----------------------------------------------------------------------------//

#include "./advanced/internal.hpp"
#include "./advanced/ApplyPackedReflectors.hpp"
#include "./advanced/Cholesky.hpp"
#include "./advanced/CholeskySolve.hpp"
#include "./advanced/GaussianElimination.hpp"
#include "./advanced/Hegst.hpp"
#include "./advanced/HermitianEig.hpp"
#include "./advanced/HermitianGenDefiniteEig.hpp"
#include "./advanced/HermitianNorm.hpp"
#include "./advanced/HermitianTridiag.hpp"
#include "./advanced/HouseholderSolve.hpp"
#include "./advanced/LDL.hpp"
#include "./advanced/LQ.hpp"
#include "./advanced/LU.hpp"
#include "./advanced/Norm.hpp"
#include "./advanced/QR.hpp"
#include "./advanced/Reflector.hpp"
#include "./advanced/SkewHermitianEig.hpp"
#include "./advanced/TriangularInverse.hpp"

template<typename F>
inline void
elemental::advanced::Cholesky
( Shape shape, Matrix<F>& A )
{
#ifndef RELEASE
    PushCallStack("advanced::Cholesky");
    if( A.Height() != A.Width() )
        throw std::logic_error( "A must be square." );
#endif
    const char uplo = ShapeToChar( shape );
    lapack::Cholesky( uplo, A.Height(), A.Buffer(), A.LDim() );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename F>
inline void
elemental::advanced::Hegst
( Side side, Shape shape, Matrix<F>& A, const Matrix<F>& B )
{
#ifndef RELEASE
    PushCallStack("advanced::Hegst");
    if( A.Height() != A.Width() )
        throw std::logic_error( "A must be square." );
    if( B.Height() != B.Width() )
        throw std::logic_error( "B must be square." );
    if( A.Height() != B.Height() )
        throw std::logic_error( "A and B must be the same size." );
#endif
    const int itype = ( side==LEFT ? 2 : 1 );
    const char uplo = ShapeToChar( shape );
    lapack::Hegst
    ( itype, uplo, A.Height(), 
      A.Buffer(), A.LDim(), B.LockedBuffer(), B.LDim() );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename F>
inline void
elemental::advanced::LU
( Matrix<F>& A, Matrix<int>& p )
{
#ifndef RELEASE
    PushCallStack("advanced::LU");
    if( p.Height() != A.Height() )
        throw std::logic_error( "A and p must be the same height." );
#endif
    lapack::LU
    ( A.Height(), A.Width(), A.Buffer(), A.LDim(), p.Buffer() );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename R>
inline void
elemental::advanced::LQ
( Matrix<R>& A )
{
#ifndef RELEASE
    PushCallStack("advanced::LQ");
#endif
    lapack::LQ( A.Height(), A.Width(), A.Buffer(), A.LDim() );
#ifndef RELEASE
    PopCallStack();
#endif
}

#ifndef WITHOUT_COMPLEX
template<typename R>
inline void
elemental::advanced::LQ
( Matrix<std::complex<R> >& A, Matrix<std::complex<R> >& t )
{
#ifndef RELEASE
    PushCallStack("advanced::LQ");
    if( t.Viewing() && 
        (t.Height() != std::min(A.Height(),A.Width()) || t.Width() != 1) )
        throw std::logic_error
              ( "t must be a vector of length equal to the min. dim. of A." );
#endif
    if( !t.Viewing() )
        t.ResizeTo( std::min(A.Height(),A.Width()), 1 );
    lapack::LQ( A.Height(), A.Width(), A.Buffer(), A.LDim(), t.Buffer() );
#ifndef RELEASE
    PopCallStack();
#endif
}
#endif

template<typename R>
inline R
elemental::advanced::SymmetricNorm
( Shape shape, const Matrix<R>& A, NormType type )
{ 
#ifndef RELEASE
    PushCallStack("advanced::SymmetricNorm");
#endif
    HermitianNorm( shape, A, type );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename R>
inline R
elemental::advanced::SymmetricNorm
( Shape shape, const DistMatrix<R,MC,MR>& A, NormType type )
{ 
#ifndef RELEASE
    PushCallStack("advanced::SymmetricNorm");
#endif
    HermitianNorm( shape, A, type );
#ifndef RELEASE
    PopCallStack();
#endif
}

#ifndef WITHOUT_COMPLEX
template<typename R>
inline R
elemental::advanced::SymmetricNorm
( Shape shape, const Matrix<std::complex<R> >& A, NormType type )
{ 
#ifndef RELEASE
    PushCallStack("advanced::SymmetricNorm");
#endif
    HermitianNorm( shape, A, type );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename R>
inline R
elemental::advanced::SymmetricNorm
( Shape shape, const DistMatrix<std::complex<R>,MC,MR>& A, NormType type )
{ 
#ifndef RELEASE
    PushCallStack("advanced::SymmetricNorm");
#endif
    HermitianNorm( shape, A, type );
#ifndef RELEASE
    PopCallStack();
#endif
}
#endif // WITHOUT_COMPLEX

template<typename R>
inline void
elemental::advanced::QR
( Matrix<R>& A )
{
#ifndef RELEASE
    PushCallStack("advanced::QR");
#endif
    lapack::QR( A.Height(), A.Width(), A.Buffer(), A.LDim() );
#ifndef RELEASE
    PopCallStack();
#endif
}

#ifndef WITHOUT_COMPLEX
template<typename R>
inline void
elemental::advanced::QR
( Matrix<std::complex<R> >& A, Matrix<std::complex<R> >& t )
{
#ifndef RELEASE
    PushCallStack("advanced::QR");
    if( t.Viewing() && 
        (t.Height() != std::min(A.Height(),A.Width()) || t.Width() != 1) )
        throw std::logic_error
              ( "t must be a vector of length equal to the min. dim. of A." );
#endif
    if( !t.Viewing() )
        t.ResizeTo( std::min(A.Height(),A.Width()), 1 );
    lapack::QR( A.Height(), A.Width(), A.Buffer(), A.LDim(), t.Buffer() );
#ifndef RELEASE
    PopCallStack();
#endif
}
#endif

template<typename R>
inline void
elemental::advanced::SVD
( Matrix<R>& A, Matrix<R>& VT, std::vector<R>& SigmaDiag )
{
#ifndef RELEASE
    PushCallStack("advanced::SVD");
#endif
    const int m = A.Height();
    const int n = A.Width();
    SigmaDiag.resize( std::min(m,n) );
    VT.ResizeTo( std::min(m,n), n );
    lapack::SVD
    ( 'O', 'S', m, n, A.Buffer(), A.LDim(), &SigmaDiag[0], 0, 0, 
      VT.Buffer(), VT.LDim() );
#ifndef RELEASE
    PopCallStack();
#endif
}

#ifndef WITHOUT_COMPLEX
template<typename R>
inline void
elemental::advanced::SVD
( Matrix<std::complex<R> >& A, Matrix<std::complex<R> >& VT, 
  std::vector<R>& SigmaDiag )
{
#ifndef RELEASE
    PushCallStack("advanced::SVD");
#endif
    const int m = A.Height();
    const int n = A.Width();
    SigmaDiag.resize( std::min(m,n) );
    VT.ResizeTo( std::min(m,n), n );
    lapack::SVD
    ( 'O', 'S', m, n, A.Buffer(), A.LDim(), &SigmaDiag[0], 0, 0, 
      VT.Buffer(), VT.LDim() );
#ifndef RELEASE
    PopCallStack();
#endif
}
#endif

template<typename R>
inline void
elemental::advanced::SVD
( Matrix<R>& A, Matrix<R>& U, Matrix<R>& VT, std::vector<R>& SigmaDiag )
{
#ifndef RELEASE
    PushCallStack("advanced::SVD");
#endif
    const int m = A.Height();
    const int n = A.Width();
    SigmaDiag.resize( std::min(m,n) );
    U.ResizeTo( m, std::min(m,n) );
    VT.ResizeTo( std::min(m,n), n );
    lapack::SVD
    ( 'S', 'S', m, n, A.Buffer(), A.LDim(), &SigmaDiag[0], U.Buffer(), U.LDim(),
      VT.Buffer(), VT.LDim() );
#ifndef RELEASE
    PopCallStack();
#endif
}

#ifndef WITHOUT_COMPLEX
template<typename R>
inline void
elemental::advanced::SVD
( Matrix<std::complex<R> >& A, Matrix<std::complex<R> >& U, 
  Matrix<std::complex<R> >& VT, std::vector<R>& SigmaDiag )
{
#ifndef RELEASE
    PushCallStack("advanced::SVD");
#endif
    const int m = A.Height();
    const int n = A.Width();
    SigmaDiag.resize( std::min(m,n) );
    U.ResizeTo( m, std::min(m,n) );
    VT.ResizeTo( std::min(m,n), n );
    lapack::SVD
    ( 'S', 'S', m, n, A.Buffer(), A.LDim(), &SigmaDiag[0], U.Buffer(), U.LDim(),
      VT.Buffer(), VT.LDim() );
#ifndef RELEASE
    PopCallStack();
#endif
}
#endif

template<typename F>
inline void
elemental::advanced::TriangularInverse
( Shape shape, Diagonal diagonal, Matrix<F>& A )
{
#ifndef RELEASE
    PushCallStack("advanced::TriangularInverse");
    if( A.Height() != A.Width() )
        throw std::logic_error( "A must be square." );
#endif
    const char uplo = ShapeToChar( shape );
    const char diag = DiagonalToChar( diagonal );
    lapack::TriangularInverse( uplo, diag, A.Height(), A.Buffer(), A.LDim() );
#ifndef RELEASE
    PopCallStack();
#endif
}

#endif /* ELEMENTAL_ADVANCED_HPP */
