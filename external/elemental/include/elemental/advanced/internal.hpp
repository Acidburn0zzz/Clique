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
#ifndef ELEMENTAL_ADVANCED_INTERNAL_HPP
#define ELEMENTAL_ADVANCED_INTERNAL_HPP 1

#include "elemental/advanced.hpp"

// Template conventions:
//   G: general datatype
//
//   T: any ring, e.g., the (Gaussian) integers and the real/complex numbers
//   Z: representation of a real ring, e.g., the integers or real numbers
//   std::complex<Z>: representation of a complex ring, e.g. Gaussian integers
//                    or complex numbers
//
//   F: representation of real or complex number
//   R: representation of real number
//   std::complex<R>: representation of complex number

namespace elemental {
namespace advanced {
namespace internal {

//----------------------------------------------------------------------------//
// Local LAPACK-like routines                                                 //
//----------------------------------------------------------------------------//

template<typename F>
void LocalCholesky( Shape shape, DistMatrix<F,STAR,STAR>& A );

template<typename F>
void LocalHegst
( Side side, Shape shape, 
  DistMatrix<F,STAR,STAR>& A, const DistMatrix<F,STAR,STAR>& B );

template<typename F>
void LocalLDL
( Orientation orientation, 
  DistMatrix<F,STAR,STAR>& A, DistMatrix<F,STAR,STAR>& d );

template<typename F>
void LocalLU( DistMatrix<F,STAR,STAR>& A );

template<typename F>
void LocalTriangularInverse
( Shape shape, Diagonal diagonal, DistMatrix<F,STAR,STAR>& A );

//----------------------------------------------------------------------------//
// Cholesky helpers                                                           //
//----------------------------------------------------------------------------//

template<typename F>
void CholeskyLVar2( DistMatrix<F,MC,MR>& A );

template<typename F>
void CholeskyLVar2Naive( DistMatrix<F,MC,MR>& A );

template<typename F>
void CholeskyLVar3( DistMatrix<F,MC,MR>& A );

template<typename F>
void CholeskyLVar3Naive( DistMatrix<F,MC,MR>& A );

template<typename F>
void CholeskyLVar3Square( DistMatrix<F,MC,MR>& A );

template<typename F>
void CholeskyUVar2( DistMatrix<F,MC,MR>& A );

template<typename F>
void CholeskyUVar2Naive( DistMatrix<F,MC,MR>& A );
 
template<typename F>
void CholeskyUVar3( DistMatrix<F,MC,MR>& A );

template<typename F>
void CholeskyUVar3Naive( DistMatrix<F,MC,MR>& A );

template<typename F>
void CholeskyUVar3Square( DistMatrix<F,MC,MR>& A );
            
//----------------------------------------------------------------------------//
// GaussElim                                                                  //
//----------------------------------------------------------------------------//
            
template<typename F>
void ReduceToRowEchelon( DistMatrix<F,MC,MR>& A, DistMatrix<F,MC,MR>& B );

//----------------------------------------------------------------------------//
// Hegst                                                                      //
//----------------------------------------------------------------------------//

template<typename F>
void HegstLLVar1( DistMatrix<F,MC,MR>& A, const DistMatrix<F,MC,MR>& L );

template<typename F>
void HegstLLVar2( DistMatrix<F,MC,MR>& A, const DistMatrix<F,MC,MR>& L );

// HegstLLVar3 would redundantly compute too much data

template<typename F>
void HegstLLVar4( DistMatrix<F,MC,MR>& A, const DistMatrix<F,MC,MR>& L );

template<typename F>
void HegstLLVar5( DistMatrix<F,MC,MR>& A, const DistMatrix<F,MC,MR>& L );

template<typename F>
void HegstLUVar1( DistMatrix<F,MC,MR>& A, const DistMatrix<F,MC,MR>& U );

template<typename F>
void HegstLUVar2( DistMatrix<F,MC,MR>& A, const DistMatrix<F,MC,MR>& U );

// HegstLUVar3 would redundantly compute too much data

template<typename F>
void HegstLUVar4( DistMatrix<F,MC,MR>& A, const DistMatrix<F,MC,MR>& U );

template<typename F>
void HegstLUVar5( DistMatrix<F,MC,MR>& A, const DistMatrix<F,MC,MR>& U );

template<typename F>
void HegstRLVar1( DistMatrix<F,MC,MR>& A, const DistMatrix<F,MC,MR>& L );

template<typename F>
void HegstRLVar2( DistMatrix<F,MC,MR>& A, const DistMatrix<F,MC,MR>& L );

template<typename F>
void HegstRLVar3( DistMatrix<F,MC,MR>& A, const DistMatrix<F,MC,MR>& L );

template<typename F>
void HegstRLVar4( DistMatrix<F,MC,MR>& A, const DistMatrix<F,MC,MR>& L );

template<typename F>
void HegstRLVar5( DistMatrix<F,MC,MR>& A, const DistMatrix<F,MC,MR>& L );

template<typename F>
void HegstRUVar1( DistMatrix<F,MC,MR>& A, const DistMatrix<F,MC,MR>& U );

template<typename F>
void HegstRUVar2( DistMatrix<F,MC,MR>& A, const DistMatrix<F,MC,MR>& U );

template<typename F>
void HegstRUVar3( DistMatrix<F,MC,MR>& A, const DistMatrix<F,MC,MR>& U );

template<typename F>
void HegstRUVar4( DistMatrix<F,MC,MR>& A, const DistMatrix<F,MC,MR>& U );

template<typename F>
void HegstRUVar5( DistMatrix<F,MC,MR>& A, const DistMatrix<F,MC,MR>& U );

//----------------------------------------------------------------------------//
// LDL                                                                        //
//----------------------------------------------------------------------------//

template<typename F>
void LDLVar3
( Orientation orientation, Matrix<F>& A, Matrix<F>& d );

template<typename F>
void LDLVar3
( Orientation orientation, DistMatrix<F,MC,MR>& A, DistMatrix<F,MC,STAR>& d );

//----------------------------------------------------------------------------//
// LU                                                                         //
//----------------------------------------------------------------------------//

template<typename F>
void ApplyRowPivots
(       DistMatrix<F,  MC,MR  >& A,
  const DistMatrix<int,VC,STAR>& p,
        int pivotOffset=0 );

template<typename F>
void ApplyRowPivots
(       DistMatrix<F,  MC,  MR  >& A,
  const DistMatrix<int,STAR,STAR>& p,
        int pivotOffset=0 );

template<typename F>
void ApplyRowPivots
(       DistMatrix<F,MC,MR>& A, 
  const std::vector<int>& image,
  const std::vector<int>& preimage,
        int pivotOffset=0 );

void ComposePivots
( const DistMatrix<int,STAR,STAR>& p,
        std::vector<int>& image,
        std::vector<int>& preimage,
        int pivotOffset = 0 );

template<typename F>
void CreatePivotOp();

template<>
void CreatePivotOp<float>();

template<>
void CreatePivotOp<double>();

#ifndef WITHOUT_COMPLEX
template<>
void CreatePivotOp<scomplex>();

template<>
void CreatePivotOp<dcomplex>();
#endif

template<typename T>
void DestroyPivotOp();

template<>
void DestroyPivotOp<float>();

template<>
void DestroyPivotOp<double>();

#ifndef WITHOUT_COMPLEX
template<>
void DestroyPivotOp<scomplex>();

template<>
void DestroyPivotOp<dcomplex>();
#endif

template<typename F>
void PanelLU
( DistMatrix<F,STAR,STAR>& A, 
  DistMatrix<F,MC,  STAR>& B, 
  DistMatrix<int,STAR,STAR>& p, 
  int pivotOffset=0 );

template<typename F>
mpi::Op PivotOp();

template<> mpi::Op PivotOp<float>();

template<> mpi::Op PivotOp<double>();

#ifndef WITHOUT_COMPLEX
template<> mpi::Op PivotOp<scomplex>();

template<> mpi::Op PivotOp<dcomplex>();
#endif
            
template<typename F>
void PivotFunc
( void* inData, void* outData, int* length, mpi::Datatype* datatype );

//----------------------------------------------------------------------------//
// LQ                                                                         //
//----------------------------------------------------------------------------//

template<typename R>
void PanelLQ( DistMatrix<R,MC,MR>& A );

#ifndef WITHOUT_COMPLEX
template<typename R>
void PanelLQ
( DistMatrix<std::complex<R>,MC,MR  >& A,
  DistMatrix<std::complex<R>,MD,STAR>& t );
#endif

//----------------------------------------------------------------------------//
// Norm                                                                       //
//----------------------------------------------------------------------------//

template<typename R>
R FrobeniusNorm( const Matrix<R>& A );
template<typename R>
R InfinityNorm( const Matrix<R>& A );
template<typename R>
R MaxNorm( const Matrix<R>& A );
template<typename R>
R OneNorm( const Matrix<R>& A );

template<typename R>
R FrobeniusNorm( const DistMatrix<R,MC,MR>& A );
template<typename R>
R InfinityNorm( const DistMatrix<R,MC,MR>& A );
template<typename R>
R MaxNorm( const DistMatrix<R,MC,MR>& A );
template<typename R>
R OneNorm( const DistMatrix<R,MC,MR>& A );

#ifndef WITHOUT_COMPLEX
template<typename R>
R FrobeniusNorm( const Matrix<std::complex<R> >& A );
template<typename R>
R InfinityNorm( const Matrix<std::complex<R> >& A );
template<typename R>
R MaxNorm( const Matrix<std::complex<R> >& A );
template<typename R>
R OneNorm( const Matrix<std::complex<R> >& A );

template<typename R>
R FrobeniusNorm( const DistMatrix<std::complex<R>,MC,MR>& A );
template<typename R>
R InfinityNorm( const DistMatrix<std::complex<R>,MC,MR>& A );
template<typename R>
R MaxNorm( const DistMatrix<std::complex<R>,MC,MR>& A );
template<typename R>
R OneNorm( const DistMatrix<std::complex<R>,MC,MR>& A );
#endif

//----------------------------------------------------------------------------//
// HermitianNorm                                                              //
//----------------------------------------------------------------------------//

template<typename R>
R HermitianFrobeniusNorm( Shape shape, const Matrix<R>& A );
template<typename R>
R HermitianInfinityNorm( Shape shape, const Matrix<R>& A );
template<typename R>
R HermitianMaxNorm( Shape shape, const Matrix<R>& A );
template<typename R>
R HermitianOneNorm( Shape shape, const Matrix<R>& A );

template<typename R>
R HermitianFrobeniusNorm( Shape shape, const DistMatrix<R,MC,MR>& A );
template<typename R>
R HermitianInfinityNorm( Shape shape, const DistMatrix<R,MC,MR>& A );
template<typename R>
R HermitianMaxNorm( Shape shape, const DistMatrix<R,MC,MR>& A );
template<typename R>
R HermitianOneNorm( Shape shape, const DistMatrix<R,MC,MR>& A );

#ifndef WITHOUT_COMPLEX
template<typename R>
R HermitianFrobeniusNorm( Shape shape, const Matrix<std::complex<R> >& A );
template<typename R>
R HermitianInfinityNorm( Shape shape, const Matrix<std::complex<R> >& A );
template<typename R>
R HermitianMaxNorm( Shape shape, const Matrix<std::complex<R> >& A );
template<typename R>
R HermitianOneNorm( Shape shape, const Matrix<std::complex<R> >& A );

template<typename R>
R HermitianFrobeniusNorm
( Shape shape, const DistMatrix<std::complex<R>,MC,MR>& A );
template<typename R>
R HermitianInfinityNorm
( Shape shape, const DistMatrix<std::complex<R>,MC,MR>& A );
template<typename R>
R HermitianMaxNorm
( Shape shape, const DistMatrix<std::complex<R>,MC,MR>& A );
template<typename R>
R HermitianOneNorm
( Shape shape, const DistMatrix<std::complex<R>,MC,MR>& A );
#endif

//----------------------------------------------------------------------------//
// QR                                                                         //
//----------------------------------------------------------------------------//

template<typename R>
void PanelQR( DistMatrix<R,MC,MR>& A );

#ifndef WITHOUT_COMPLEX
template<typename R>
void PanelQR
( DistMatrix<std::complex<R>,MC,MR  >& A,
  DistMatrix<std::complex<R>,MD,STAR>& t );
#endif

//----------------------------------------------------------------------------//
// Reflector                                                                  //
//----------------------------------------------------------------------------//
template<typename R>
R ColReflector( DistMatrix<R,MC,MR>& chi, DistMatrix<R,MC,MR>& x );

#ifndef WITHOUT_COMPLEX
template<typename R>
std::complex<R> ColReflector
( DistMatrix<std::complex<R>,MC,MR>& chi, 
  DistMatrix<std::complex<R>,MC,MR>& x );
#endif

template<typename R>
R RowReflector( DistMatrix<R,MC,MR>& chi, DistMatrix<R,MC,MR>& x );

#ifndef WITHOUT_COMPLEX
template<typename R>
std::complex<R> RowReflector
( DistMatrix<std::complex<R>,MC,MR>& chi,
  DistMatrix<std::complex<R>,MC,MR>& x );
#endif

//----------------------------------------------------------------------------//
// HermitianTridiag                                                           //
//----------------------------------------------------------------------------//

template<typename R>
void HermitianPanelTridiagL
( DistMatrix<R,MC,MR  >& A, 
  DistMatrix<R,MC,MR  >& W,
  DistMatrix<R,MC,STAR>& APan_MC_STAR,
  DistMatrix<R,MR,STAR>& APan_MR_STAR,
  DistMatrix<R,MC,STAR>& W_MC_STAR,
  DistMatrix<R,MR,STAR>& W_MR_STAR );
template<typename R>
void HermitianPanelTridiagU
( DistMatrix<R,MC,MR  >& A, 
  DistMatrix<R,MC,MR  >& W,
  DistMatrix<R,MC,STAR>& APan_MC_STAR,
  DistMatrix<R,MR,STAR>& APan_MR_STAR,
  DistMatrix<R,MC,STAR>& W_MC_STAR,
  DistMatrix<R,MR,STAR>& W_MR_STAR );
template<typename R>
void HermitianPanelTridiagLSquare
( DistMatrix<R,MC,MR  >& A, 
  DistMatrix<R,MC,MR  >& W,
  DistMatrix<R,MC,STAR>& APan_MC_STAR,
  DistMatrix<R,MR,STAR>& APan_MR_STAR,
  DistMatrix<R,MC,STAR>& W_MC_STAR,
  DistMatrix<R,MR,STAR>& W_MR_STAR );
template<typename R>
void HermitianPanelTridiagUSquare
( DistMatrix<R,MC,MR  >& A, 
  DistMatrix<R,MC,MR  >& W,
  DistMatrix<R,MC,STAR>& APan_MC_STAR,
  DistMatrix<R,MR,STAR>& APan_MR_STAR,
  DistMatrix<R,MC,STAR>& W_MC_STAR,
  DistMatrix<R,MR,STAR>& W_MR_STAR );

#ifndef WITHOUT_COMPLEX
template<typename R>
void HermitianPanelTridiagL
( DistMatrix<std::complex<R>,MC,MR  >& A,
  DistMatrix<std::complex<R>,MC,MR  >& W,
  DistMatrix<std::complex<R>,MD,STAR>& t,
  DistMatrix<std::complex<R>,MC,STAR>& APan_MC_STAR,
  DistMatrix<std::complex<R>,MR,STAR>& APan_MR_STAR,
  DistMatrix<std::complex<R>,MC,STAR>& W_MC_STAR,
  DistMatrix<std::complex<R>,MR,STAR>& W_MR_STAR );
template<typename R>
void HermitianPanelTridiagU
( DistMatrix<std::complex<R>,MC,MR  >& A,
  DistMatrix<std::complex<R>,MC,MR  >& W,
  DistMatrix<std::complex<R>,MD,STAR>& t,
  DistMatrix<std::complex<R>,MC,STAR>& APan_MC_STAR,
  DistMatrix<std::complex<R>,MR,STAR>& APan_MR_STAR,
  DistMatrix<std::complex<R>,MC,STAR>& W_MC_STAR,
  DistMatrix<std::complex<R>,MR,STAR>& W_MR_STAR );

template<typename R>
void HermitianPanelTridiagLSquare
( DistMatrix<std::complex<R>,MC,MR  >& A,
  DistMatrix<std::complex<R>,MC,MR  >& W,
  DistMatrix<std::complex<R>,MD,STAR>& t,
  DistMatrix<std::complex<R>,MC,STAR>& APan_MC_STAR,
  DistMatrix<std::complex<R>,MR,STAR>& APan_MR_STAR,
  DistMatrix<std::complex<R>,MC,STAR>& W_MC_STAR,
  DistMatrix<std::complex<R>,MR,STAR>& W_MR_STAR );
template<typename R>
void HermitianPanelTridiagUSquare
( DistMatrix<std::complex<R>,MC,MR  >& A,
  DistMatrix<std::complex<R>,MC,MR  >& W,
  DistMatrix<std::complex<R>,MD,STAR>& t,
  DistMatrix<std::complex<R>,MC,STAR>& APan_MC_STAR,
  DistMatrix<std::complex<R>,MR,STAR>& APan_MR_STAR,
  DistMatrix<std::complex<R>,MC,STAR>& W_MC_STAR,
  DistMatrix<std::complex<R>,MR,STAR>& W_MR_STAR );
#endif
 
template<typename R>
void HermitianTridiagL( DistMatrix<R,MC,MR>& A );
template<typename R>
void HermitianTridiagU( DistMatrix<R,MC,MR>& A );

template<typename R>
void HermitianTridiagLSquare( DistMatrix<R,MC,MR>& A );
template<typename R>
void HermitianTridiagUSquare( DistMatrix<R,MC,MR>& A );

#ifndef WITHOUT_COMPLEX
template<typename R>
void HermitianTridiagL
( DistMatrix<std::complex<R>,MC,  MR  >& A, 
  DistMatrix<std::complex<R>,STAR,STAR>& t );
template<typename R>
void HermitianTridiagU
( DistMatrix<std::complex<R>,MC,  MR  >& A, 
  DistMatrix<std::complex<R>,STAR,STAR>& t );

template<typename R>
void HermitianTridiagLSquare
( DistMatrix<std::complex<R>,MC,  MR  >& A, 
  DistMatrix<std::complex<R>,STAR,STAR>& t );
template<typename R>
void HermitianTridiagUSquare
( DistMatrix<std::complex<R>,MC,  MR  >& A, 
  DistMatrix<std::complex<R>,STAR,STAR>& t );
#endif

//----------------------------------------------------------------------------//
// Triangular Inverse                                                         //
//----------------------------------------------------------------------------//

template<typename F>
void TriangularInverseVar3
( Shape shape, Diagonal diagonal, DistMatrix<F,MC,MR>& A  );

template<typename F>
void TriangularInverseLVar3
( Diagonal diagonal, DistMatrix<F,MC,MR>& L );

template<typename F>
void TriangularInverseUVar3
( Diagonal diagonal, DistMatrix<F,MC,MR>& U );

//----------------------------------------------------------------------------//
// ApplyPackedReflectors                                                      //
//----------------------------------------------------------------------------//

template<typename R>
void ApplyPackedReflectorsLLVF
( int offset, 
  const DistMatrix<R,MC,MR>& H, 
        DistMatrix<R,MC,MR>& A );
#ifndef WITHOUT_COMPLEX
template<typename R>
void ApplyPackedReflectorsLLVF
( Conjugation conjugation, int offset,
  const DistMatrix<std::complex<R>,MC,MR  >& H,
  const DistMatrix<std::complex<R>,MD,STAR>& t,
        DistMatrix<std::complex<R>,MC,MR  >& A );
#endif

template<typename R>
void ApplyPackedReflectorsLLVB
( int offset, 
  const DistMatrix<R,MC,MR>& H, 
        DistMatrix<R,MC,MR>& A );
#ifndef WITHOUT_COMPLEX
template<typename R>
void ApplyPackedReflectorsLLVB
( Conjugation conjugation, int offset,
  const DistMatrix<std::complex<R>,MC,MR  >& H,
  const DistMatrix<std::complex<R>,MD,STAR>& t,
        DistMatrix<std::complex<R>,MC,MR  >& A );
#endif

template<typename R>
void ApplyPackedReflectorsLLHF
( int offset, 
  const DistMatrix<R,MC,MR>& H, 
        DistMatrix<R,MC,MR>& A );
#ifndef WITHOUT_COMPLEX
template<typename R>
void ApplyPackedReflectorsLLHF
( Conjugation conjugation, int offset,
  const DistMatrix<std::complex<R>,MC,MR  >& H,
  const DistMatrix<std::complex<R>,MD,STAR>& t,
        DistMatrix<std::complex<R>,MC,MR  >& A );
#endif

template<typename R>
void ApplyPackedReflectorsLLHB
( int offset, 
  const DistMatrix<R,MC,MR>& H, 
        DistMatrix<R,MC,MR>& A );
#ifndef WITHOUT_COMPLEX
template<typename R>
void ApplyPackedReflectorsLLHB
( Conjugation conjugation, int offset,
  const DistMatrix<std::complex<R>,MC,MR  >& H,
  const DistMatrix<std::complex<R>,MD,STAR>& t,
        DistMatrix<std::complex<R>,MC,MR  >& A );
#endif

template<typename R>
void ApplyPackedReflectorsLUVF
( int offset, 
  const DistMatrix<R,MC,MR>& H, 
        DistMatrix<R,MC,MR>& A );
#ifndef WITHOUT_COMPLEX
template<typename R>
void ApplyPackedReflectorsLUVF
( Conjugation conjugation, int offset,
  const DistMatrix<std::complex<R>,MC,MR  >& H,
  const DistMatrix<std::complex<R>,MD,STAR>& t,
        DistMatrix<std::complex<R>,MC,MR  >& A );
#endif

template<typename R>
void ApplyPackedReflectorsLUVB
( int offset, 
  const DistMatrix<R,MC,MR>& H, 
        DistMatrix<R,MC,MR>& A );
#ifndef WITHOUT_COMPLEX
template<typename R>
void ApplyPackedReflectorsLUVB
( Conjugation conjugation, int offset,
  const DistMatrix<std::complex<R>,MC,MR  >& H,
  const DistMatrix<std::complex<R>,MD,STAR>& t,
        DistMatrix<std::complex<R>,MC,MR  >& A );
#endif

template<typename R>
void ApplyPackedReflectorsLUHF
( int offset, 
  const DistMatrix<R,MC,MR>& H, 
        DistMatrix<R,MC,MR>& A );
#ifndef WITHOUT_COMPLEX
template<typename R>
void ApplyPackedReflectorsLUHF
( Conjugation conjugation, int offset,
  const DistMatrix<std::complex<R>,MC,MR  >& H,
  const DistMatrix<std::complex<R>,MD,STAR>& t,
        DistMatrix<std::complex<R>,MC,MR  >& A );
#endif

template<typename R>
void ApplyPackedReflectorsLUHB
( int offset, 
  const DistMatrix<R,MC,MR>& H, 
        DistMatrix<R,MC,MR>& A );
#ifndef WITHOUT_COMPLEX
template<typename R>
void ApplyPackedReflectorsLUHB
( Conjugation conjugation, int offset,
  const DistMatrix<std::complex<R>,MC,MR  >& H,
  const DistMatrix<std::complex<R>,MD,STAR>& t,
        DistMatrix<std::complex<R>,MC,MR  >& A );
#endif

template<typename R>
void ApplyPackedReflectorsRLVF
( int offset, 
  const DistMatrix<R,MC,MR>& H, 
        DistMatrix<R,MC,MR>& A );
#ifndef WITHOUT_COMPLEX
template<typename R>
void ApplyPackedReflectorsRLVF
( Conjugation conjugation, int offset,
  const DistMatrix<std::complex<R>,MC,MR  >& H,
  const DistMatrix<std::complex<R>,MD,STAR>& t,
        DistMatrix<std::complex<R>,MC,MR  >& A );
#endif

template<typename R>
void ApplyPackedReflectorsRLVB
( int offset, 
  const DistMatrix<R,MC,MR>& H, 
        DistMatrix<R,MC,MR>& A );
#ifndef WITHOUT_COMPLEX
template<typename R>
void ApplyPackedReflectorsRLVB
( Conjugation conjugation, int offset,
  const DistMatrix<std::complex<R>,MC,MR  >& H,
  const DistMatrix<std::complex<R>,MD,STAR>& t,
        DistMatrix<std::complex<R>,MC,MR  >& A );
#endif

template<typename R>
void ApplyPackedReflectorsRLHF
( int offset, 
  const DistMatrix<R,MC,MR>& H, 
        DistMatrix<R,MC,MR>& A );
#ifndef WITHOUT_COMPLEX
template<typename R>
void ApplyPackedReflectorsRLHF
( Conjugation conjugation, int offset,
  const DistMatrix<std::complex<R>,MC,MR  >& H,
  const DistMatrix<std::complex<R>,MD,STAR>& t,
        DistMatrix<std::complex<R>,MC,MR  >& A );
#endif

template<typename R>
void ApplyPackedReflectorsRLHB
( int offset, 
  const DistMatrix<R,MC,MR>& H, 
        DistMatrix<R,MC,MR>& A );
#ifndef WITHOUT_COMPLEX
template<typename R>
void ApplyPackedReflectorsRLHB
( Conjugation conjugation, int offset,
  const DistMatrix<std::complex<R>,MC,MR  >& H,
  const DistMatrix<std::complex<R>,MD,STAR>& t,
        DistMatrix<std::complex<R>,MC,MR  >& A );
#endif

template<typename R>
void ApplyPackedReflectorsRUVF
( int offset, 
  const DistMatrix<R,MC,MR>& H, 
        DistMatrix<R,MC,MR>& A );
#ifndef WITHOUT_COMPLEX
template<typename R>
void ApplyPackedReflectorsRUVF
( Conjugation conjugation, int offset,
  const DistMatrix<std::complex<R>,MC,MR  >& H,
  const DistMatrix<std::complex<R>,MD,STAR>& t,
        DistMatrix<std::complex<R>,MC,MR  >& A );
#endif

template<typename R>
void ApplyPackedReflectorsRUVB
( int offset, 
  const DistMatrix<R,MC,MR>& H, 
        DistMatrix<R,MC,MR>& A );
#ifndef WITHOUT_COMPLEX
template<typename R>
void ApplyPackedReflectorsRUVB
( Conjugation conjugation, int offset,
  const DistMatrix<std::complex<R>,MC,MR  >& H,
  const DistMatrix<std::complex<R>,MD,STAR>& t,
        DistMatrix<std::complex<R>,MC,MR  >& A );
#endif

template<typename R>
void ApplyPackedReflectorsRUHF
( int offset, 
  const DistMatrix<R,MC,MR>& H, 
        DistMatrix<R,MC,MR>& A );
#ifndef WITHOUT_COMPLEX
template<typename R>
void ApplyPackedReflectorsRUHF
( Conjugation conjugation, int offset,
  const DistMatrix<std::complex<R>,MC,MR  >& H,
  const DistMatrix<std::complex<R>,MD,STAR>& t,
        DistMatrix<std::complex<R>,MC,MR  >& A );
#endif

template<typename R>
void ApplyPackedReflectorsRUHB
( int offset, 
  const DistMatrix<R,MC,MR>& H, 
        DistMatrix<R,MC,MR>& A );
#ifndef WITHOUT_COMPLEX
template<typename R>
void ApplyPackedReflectorsRUHB
( Conjugation conjugation, int offset,
  const DistMatrix<std::complex<R>,MC,MR  >& H,
  const DistMatrix<std::complex<R>,MD,STAR>& t,
        DistMatrix<std::complex<R>,MC,MR  >& A );
#endif

//----------------------------------------------------------------------------//
// LAPACK-like Utility Functions                                              //
//----------------------------------------------------------------------------//
template<typename F>
double CholeskyGFlops( int m, double seconds );

template<typename F>
double HegstGFlops( int m, double seconds );

template<typename F>
double LDLHGFlops( int m, double seconds );

template<typename F>
double LDLTGFlops( int m, double seconds );

template<typename F>
double LUGFlops( int m, double seconds );

template<typename F>
double LQGFlops( int m, int n, double seconds );

template<typename F>
double QRGFlops( int m, int n, double seconds );

template<typename F>
double HermitianTridiagGFlops( int m, double seconds );

template<typename F>
double TriangularInverseGFlops( int m, double seconds );

template<typename F>
double ApplyPackedReflectorsGFlops( int m, double seconds );

} // internal
} // advanced
} // elemental

//----------------------------------------------------------------------------//
// Implementation begins here                                                 //
//----------------------------------------------------------------------------//

namespace elemental {
namespace advanced {
namespace internal {

//
// Local LAPACK-like routines
//

template<typename F>
inline void
LocalCholesky
( Shape shape, DistMatrix<F,STAR,STAR>& A )
{
#ifndef RELEASE
    PushCallStack("advanced::internal::LocalCholesky");
#endif
    Cholesky( shape, A.LocalMatrix() );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename F>
inline void
LocalHegst
( Side side, Shape shape,
  DistMatrix<F,STAR,STAR>& A, const DistMatrix<F,STAR,STAR>& B )
{
#ifndef RELEASE
    PushCallStack("advanced::internal::LocalHegst");
#endif
    Hegst( side, shape, A.LocalMatrix(), B.LockedLocalMatrix() );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename F>
inline void
LocalLDL
( Orientation orientation, 
  DistMatrix<F,STAR,STAR>& A, DistMatrix<F,STAR,STAR>& d )
{
#ifndef RELEASE
    PushCallStack("advanced::internal::LocalLDL");
    if( d.Viewing() && (d.Height() != A.Height() || d.Width() != 1) )
        throw std::logic_error
        ("d must be a column vector of the same height as A");
#endif
    if( !d.Viewing() )
        d.ResizeTo( A.Height(), 1 );
    advanced::internal::LDLVar3
    ( orientation, A.LocalMatrix(), d.LocalMatrix() );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename F>
inline void
LocalLU( DistMatrix<F,STAR,STAR>& A )
{
#ifndef RELEASE
    PushCallStack("advanced::internal::LocalLU");
#endif
    advanced::LU( A.LocalMatrix() );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename F>
inline void
LocalTriangularInverse
( Shape shape, Diagonal diagonal, DistMatrix<F,STAR,STAR>& A )
{ 
#ifndef RELEASE
    PushCallStack("advanced::internal::LocalTriangularInverse");
#endif
    TriangularInverse( shape, diagonal, A.LocalMatrix() );
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// GFlop helpers
//

template<>
inline double
CholeskyGFlops<float>
( int m, double seconds )
{ return (1./3.*m*m*m)/(1.e9*seconds); }
            
template<>
inline double
CholeskyGFlops<double>
( int m, double seconds )
{ return CholeskyGFlops<float>(m,seconds); }
            
#ifndef WITHOUT_COMPLEX
template<>
inline double
CholeskyGFlops<scomplex>
( int m, double seconds )
{ return 4.*CholeskyGFlops<float>(m,seconds); }
            
template<>
inline double
CholeskyGFlops<dcomplex>
( int m, double seconds )
{ return 4.*CholeskyGFlops<float>(m,seconds); }
#endif

template<>
inline double
HegstGFlops<float>
( int m, double seconds )
{ return (1.*m*m*m)/(1.e9*seconds); }

template<>
inline double
HegstGFlops<double>
( int m, double seconds )
{ return HegstGFlops<float>(m,seconds); }

#ifndef WITHOUT_COMPLEX
template<>
inline double
HegstGFlops<scomplex>
( int m, double seconds )
{ return 4.*HegstGFlops<float>(m,seconds); }

template<>
inline double
HegstGFlops<dcomplex>
( int m, double seconds )
{ return 4.*HegstGFlops<float>(m,seconds); }
#endif

template<>
inline double
LUGFlops<float>
( int m, double seconds )
{ return (2./3.*m*m*m)/(1.e9*seconds); }

template<typename F>
inline double LDLHGFlops( int n, double seconds )
{ return CholeskyGFlops<F>( n, seconds ); }

template<typename F>
inline double LDLTGFlops( int n, double seconds )
{ return CholeskyGFlops<F>( n, seconds ); }

template<>
inline double
LUGFlops<double>
( int m, double seconds )
{ return LUGFlops<float>(m,seconds); }

#ifndef WITHOUT_COMPLEX
template<>
inline double
LUGFlops<scomplex>
( int m, double seconds )
{ return 4.*LUGFlops<float>(m,seconds); }

template<>
inline double
LUGFlops<dcomplex>
( int m, double seconds )
{ return 4.*LUGFlops<float>(m,seconds); }
#endif

template<>
inline double
LQGFlops<float>
( int m, int n, double seconds )
{ return (2.*m*m*n-2./3.*m*m*m)/(1.e9*seconds); }

template<>
inline double
LQGFlops<double>
( int m, int n, double seconds )
{ return LQGFlops<float>(m,n,seconds); }

#ifndef WITHOUT_COMPLEX
template<>
inline double
LQGFlops<scomplex>
( int m, int n, double seconds )
{ return 4.*LQGFlops<float>(m,n,seconds); }

template<>
inline double
LQGFlops<dcomplex>
( int m, int n, double seconds )
{ return 4.*LQGFlops<float>(m,n,seconds); }
#endif

template<>
inline double
QRGFlops<float>
( int m, int n, double seconds )
{ return (2.*m*n*n-2./3.*n*n*n)/(1.e9*seconds); }

template<>
inline double
QRGFlops<double>
( int m, int n, double seconds )
{ return QRGFlops<float>(m,n,seconds); }

#ifndef WITHOUT_COMPLEX
template<>
inline double
QRGFlops<scomplex>
( int m, int n, double seconds )
{ return 4.*QRGFlops<float>(m,n,seconds); }

template<>
inline double
QRGFlops<dcomplex>
( int m, int n, double seconds )
{ return 4.*QRGFlops<float>(m,n,seconds); }
#endif

template<>
inline double
HermitianTridiagGFlops<float>
( int m, double seconds )
{ return (4./3.*m*m*m)/(1.e9*seconds); }

template<>
inline double
HermitianTridiagGFlops<double>
( int m, double seconds )
{ return HermitianTridiagGFlops<float>(m,seconds); }

#ifndef WITHOUT_COMPLEX
template<>
inline double
HermitianTridiagGFlops<scomplex>
( int m, double seconds )
{ return 4.*HermitianTridiagGFlops<float>(m,seconds); }

template<>
inline double
HermitianTridiagGFlops<dcomplex>
( int m, double seconds )
{ return 4.*HermitianTridiagGFlops<float>(m,seconds); }
#endif

template<>
inline double
TriangularInverseGFlops<float>
( int m, double seconds )
{ return (1./3.*m*m*m)/(1.e9*seconds); }

template<>
inline double
TriangularInverseGFlops<double>
( int m, double seconds )
{ return TriangularInverseGFlops<float>(m,seconds); }

#ifndef WITHOUT_COMPLEX
template<>
inline double
TriangularInverseGFlops<scomplex>
( int m, double seconds )
{ return 4.*TriangularInverseGFlops<float>(m,seconds); }

template<>
inline double
TriangularInverseGFlops<dcomplex>
( int m, double seconds )
{ return 4.*TriangularInverseGFlops<float>(m,seconds); }
#endif

template<>
inline double
ApplyPackedReflectorsGFlops<float>
( int m, double seconds )
{ return (2.*m*m*m)/(1.e9*seconds); }

template<>
inline double
ApplyPackedReflectorsGFlops<double>
( int m, double seconds )
{ return ApplyPackedReflectorsGFlops<float>(m,seconds); }

#ifndef WITHOUT_COMPLEX
template<>
inline double
ApplyPackedReflectorsGFlops<scomplex>
( int m, double seconds )
{ return 4.*ApplyPackedReflectorsGFlops<float>(m,seconds); }

template<>
inline double
ApplyPackedReflectorsGFlops<dcomplex>
( int m, double seconds )
{ return 4.*ApplyPackedReflectorsGFlops<float>(m,seconds); }
#endif

} // internal
} // advanced
} // elemental

#endif /* ELEMENTAL_ADVANCED_INTERNAL_HPP */
