Frontal trees
=============

DistSymmFrontTree
-----------------

This data structure represents a distributed symmetric :math:`LDL^T` or 
:math:`LDL^H` factorization.

.. cpp:type:: struct SymmFront<T>

   .. cpp:member:: Matrix<T> frontL

      The left half of a local symmetric frontal matrix. If `frontL` is 
      :math:`m \times n`, then the associated node has :math:`n` vertices 
      and the lower structure is of size :math:`m-n`.

   .. cpp:member:: Matrix<T> diag

      Stores the diagonal of the frontal matrix after factorization.

.. cpp:type:: struct DistSymmFront<T>

   .. cpp:member:: DistMatrix<T,VC,STAR> front1dL
   .. cpp:member:: DistMatrix<T> front2dL

      The left half of a frontal matrix in a 1D/2D distribution. 
      If `front1dL`/`front2dL` is :math:`m \times n`, then the associated node 
      has :math:`n` vertices and the lower structure is of size :math:`m-n`.

   .. cpp:member:: DistMatrix<T,VC,STAR> diag1d

      Stores the diagonal of the frontal matrix after factorization.

.. cpp:type:: enum SymmFrontType

   Can be set to either
   
   * ``SYMM_1D``: Symmetric/Hermitian fronts distributed in a 1D manner

   * ``SYMM_2D``: Symmetric/Hermitian fronts distributed in a 2D manner 

   * ``LDL_1D``: LDL factorization distributed in a 1D manner

   * ``LDL_2D``: LDL factorization distributed in a 2D manner 

   * ``LDL_SELINV_1D``: LDL factorization with inverted diagonal blocks 
     distributed in a 1D manner

   * ``LDL_SELINV_2D``: LDL factorization with inverted diagonal blocks 
     distributed in a 2D manner

   * ``BLOCK_LDL_2D``: Block LDL factorization with fronts distributed in a 
     2D manner

.. cpp:type:: struct DistSymmFrontTree<T>

   .. cpp:member:: bool isHermitian

      If true, the matrix is assumed to be Hermitian; otherwise, it is 
      treated as symmetric.

   .. cpp:member:: SymmFrontType frontType

      Specifies the form of the frontal matrices.

   .. cpp:member:: std::vector<SymmFront<T> > localFronts

      The vector of local frontal matrices.

   .. cpp:member:: std::vector<DistSymmFront<T> > distFronts

      The vector of distributed frontal matrices.

   .. cpp:function:: DistSymmFrontTree( Orientation orientation, const DistSparseMatrix<T>& A, const DistMap& map, const DistSeparatorTree& sepTree, const DistSymmInfo& info )

      A constructor which converts a distributed sparse matrix into a symmetric
      frontal tree which is ready for factorization (e.g., with 
      :cpp:func:`LDL` or :cpp:func:`BlockLDL`).

   .. cpp:function:: void Initialize( Orientation orientation, const DistSparseMatrix<T>& A, const DistMap& map, const DistSeparatorTree& sepTree, const DistSymmInfo& info )

      The same as the :cpp:func:`DistSymmFrontTree\<T>::DistSymmFrontTree`
      constructor, but callable after construction.

   .. cpp:function:: void TopLeftMemoryInfo( double& numLocalEntries, double& minLocalEntries, double& maxLocalEntries, double& numGlobalEntries ) const

   .. cpp:function:: void BottomLeftMemoryInfo( double& numLocalEntries, double& minLocalEntries, double& maxLocalEntries, double& numGlobalEntries ) const

   .. cpp:function:: void MemoryInfo( double& numLocalEntries, double& minLocalEntries, double& maxLocalEntries, double& numGlobalEntries ) const

   .. cpp:function:: void FactorizationWork( double& numLocalFlops, double& minLocalFlops, double& maxLocalFlops, double& numGlobalFlops, bool selInv=false ) const

   .. cpp:function:: void SolveWork( double& numLocalFlops, double& minLocalFlops, double& maxLocalFlops, double& numGlobalFlops, int numRhs=1 ) const

.. cpp:type:: struct DistSymmFrontTree<F>

   Same as above, but this implies that the underlying datatype `F` is a field.