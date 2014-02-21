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
#ifndef CLIQ_CORE_DISTGRAPH_IMPL_HPP
#define CLIQ_CORE_DISTGRAPH_IMPL_HPP

namespace cliq {

inline 
DistGraph::DistGraph()
: numSources_(0), numTargets_(0), comm_(mpi::COMM_WORLD)
{ SetComm( mpi::COMM_WORLD ); }

inline
DistGraph::DistGraph( mpi::Comm comm )
: numSources_(0), numTargets_(0), comm_(mpi::COMM_WORLD)
{ SetComm( comm ); }

inline 
DistGraph::DistGraph( int numVertices, mpi::Comm comm )
: numSources_(numVertices), numTargets_(numVertices), comm_(mpi::COMM_WORLD)
{ SetComm( comm ); }

inline 
DistGraph::DistGraph( int numSources, int numTargets, mpi::Comm comm )
: numSources_(numSources), numTargets_(numTargets), comm_(mpi::COMM_WORLD)
{ SetComm( comm ); }

inline
DistGraph::DistGraph( const Graph& graph )
{
    DEBUG_ONLY(CallStackEntry cse("DistGraph::DistGraph"))
    *this = graph;
}

inline
DistGraph::DistGraph( const DistGraph& graph )
{
    DEBUG_ONLY(CallStackEntry cse("DistGraph::DistGraph"))
    if( &graph != this )
        *this = graph;
    else
        LogicError("Tried to construct DistGraph with itself");
}

inline
DistGraph::~DistGraph()
{ 
    if( comm_ != mpi::COMM_WORLD )
        mpi::CommFree( comm_ );
} 

inline int 
DistGraph::NumSources() const
{ return numSources_; }

inline int 
DistGraph::NumTargets() const
{ return numTargets_; }

inline void 
DistGraph::SetComm( mpi::Comm comm )
{
    if( comm_ != mpi::COMM_WORLD )
        mpi::CommFree( comm_ );

    SwapClear( sources_ );
    SwapClear( targets_ );
    sorted_ = true;
    assembling_ = false;
    SwapClear( localEdgeOffsets_ );

    if( comm == mpi::COMM_WORLD )
        comm_ = comm;
    else
        mpi::CommDup( comm, comm_ );

    const int commRank = mpi::CommRank( comm );
    const int commSize = mpi::CommSize( comm );
    blocksize_ = numSources_/commSize;
    firstLocalSource_ = commRank*blocksize_;
    if( commRank < commSize-1 )
        numLocalSources_ = blocksize_;
    else
        numLocalSources_ = numSources_ - (commSize-1)*blocksize_;
}

inline mpi::Comm 
DistGraph::Comm() const
{ return comm_; }

inline int
DistGraph::Blocksize() const
{ return blocksize_; }

inline int
DistGraph::FirstLocalSource() const
{ return firstLocalSource_; }

inline int
DistGraph::NumLocalSources() const
{ return numLocalSources_; }

inline int
DistGraph::NumLocalEdges() const
{
    DEBUG_ONLY(
        CallStackEntry cse("DistGraph::NumLocalEdges");
        EnsureConsistentSizes();
    )
    return sources_.size();
}

inline int
DistGraph::Capacity() const
{
    DEBUG_ONLY(
        CallStackEntry cse("DistGraph::Capacity");
        EnsureConsistentSizes();
        EnsureConsistentCapacities();
    )
    return sources_.capacity();
}

inline int
DistGraph::Source( int localEdge ) const
{
    DEBUG_ONLY(
        CallStackEntry cse("DistGraph::Source");
        if( localEdge < 0 || localEdge >= (int)sources_.size() )
            LogicError("Edge number out of bounds");
    )
    EnsureNotAssembling();
    return sources_[localEdge];
}

inline int
DistGraph::Target( int localEdge ) const
{
    DEBUG_ONLY(
        CallStackEntry cse("DistGraph::Target");
        if( localEdge < 0 || localEdge >= (int)targets_.size() )
            LogicError("Edge number out of bounds");
    )
    EnsureNotAssembling();
    return targets_[localEdge];
}

inline int
DistGraph::LocalEdgeOffset( int localSource ) const
{
    DEBUG_ONLY(
        CallStackEntry cse("DistGraph::LocalEdgeOffset");
        if( localSource < 0 || localSource > numLocalSources_ )
            LogicError
            ("Out of bounds localSource: ",localSource,
             " is not in [0,",numLocalSources_,")");
    )
    EnsureNotAssembling();
    return localEdgeOffsets_[localSource];
}

inline int
DistGraph::NumConnections( int localSource ) const
{
    DEBUG_ONLY(CallStackEntry cse("DistGraph::NumConnections"))
    return LocalEdgeOffset(localSource+1) - LocalEdgeOffset(localSource);
}

inline int*
DistGraph::SourceBuffer()
{ return &sources_[0]; }

inline int*
DistGraph::TargetBuffer()
{ return &targets_[0]; }

inline const int*
DistGraph::LockedSourceBuffer() const
{ return &sources_[0]; }

inline const int*
DistGraph::LockedTargetBuffer() const
{ return &targets_[0]; }

inline const DistGraph&
DistGraph::operator=( const Graph& graph )
{
    DEBUG_ONLY(CallStackEntry cse("DistGraph::operator="))
    numSources_ = graph.numSources_; 
    numTargets_ = graph.numTargets_;

    SetComm( mpi::COMM_SELF );

    sources_ = graph.sources_;
    targets_ = graph.targets_;

    sorted_ = graph.sorted_;
    assembling_ = graph.assembling_;
    localEdgeOffsets_ = graph.edgeOffsets_;
    return *this;
}

inline const DistGraph& 
DistGraph::operator=( const DistGraph& graph )
{
    DEBUG_ONLY(CallStackEntry cse("DistGraph::operator="))
    numSources_ = graph.numSources_;
    numTargets_ = graph.numTargets_;

    SetComm( graph.comm_ );

    sources_ = graph.sources_;
    targets_ = graph.targets_;

    sorted_ = graph.sorted_;
    assembling_ = graph.assembling_;
    localEdgeOffsets_ = graph.localEdgeOffsets_;
    return *this;
}

inline bool
DistGraph::ComparePairs
( const std::pair<int,int>& a, const std::pair<int,int>& b )
{ return a.first < b.first || (a.first == b.first && a.second < b.second); }

inline void
DistGraph::StartAssembly()
{
    DEBUG_ONLY(CallStackEntry cse("DistGraph::StartAssembly"))
    EnsureNotAssembling();
    assembling_ = true;
}

inline void
DistGraph::StopAssembly()
{
    DEBUG_ONLY(CallStackEntry cse("DistGraph::StopAssembly"))
    if( !assembling_ )
        LogicError("Cannot stop assembly without starting");
    assembling_ = false;

    // Ensure that the connection pairs are sorted
    if( !sorted_ )
    {
        const int numLocalEdges = sources_.size();
        std::vector<std::pair<int,int> > pairs( numLocalEdges );
        for( int e=0; e<numLocalEdges; ++e )
        {
            pairs[e].first = sources_[e];
            pairs[e].second = targets_[e];
        }
        std::sort( pairs.begin(), pairs.end(), ComparePairs );

        // Compress out duplicates
        int lastUnique=0;
        for( int e=1; e<numLocalEdges; ++e )
            if( pairs[e] != pairs[lastUnique] )
                pairs[++lastUnique] = pairs[e];
        const int numUnique = lastUnique+1;

        sources_.resize( numUnique );
        targets_.resize( numUnique );
        for( int e=0; e<numUnique; ++e )
        {
            sources_[e] = pairs[e].first;
            targets_[e] = pairs[e].second;
        }
    }

    ComputeLocalEdgeOffsets();
}

inline void
DistGraph::ComputeLocalEdgeOffsets()
{
    DEBUG_ONLY(CallStackEntry cse("DistGraph::ComputeLocalEdgeOffsets"))
    // Compute the local edge offsets
    int sourceOffset = 0;
    int prevSource = firstLocalSource_-1;
    localEdgeOffsets_.resize( numLocalSources_+1 );
    const int numLocalEdges = NumLocalEdges();
    for( int localEdge=0; localEdge<numLocalEdges; ++localEdge )
    {
        const int source = Source( localEdge );
        DEBUG_ONLY(
            if( source < prevSource )
                RuntimeError("sources were not properly sorted");
        )
        while( source != prevSource )
        {
            localEdgeOffsets_[sourceOffset++] = localEdge;
            ++prevSource;
        }
    }
    localEdgeOffsets_[numLocalSources_] = numLocalEdges;
}

inline void
DistGraph::Reserve( int numLocalEdges )
{ 
    sources_.reserve( numLocalEdges );
    targets_.reserve( numLocalEdges );
}

inline void
DistGraph::Insert( int source, int target )
{
    DEBUG_ONLY(
        CallStackEntry cse("DistGraph::Insert");
        EnsureConsistentSizes();
        const int capacity = Capacity();
        const int numLocalEdges = NumLocalEdges();
        if( source < firstLocalSource_ || 
            source >= firstLocalSource_+numLocalSources_ )
            LogicError
            ("Source was out of bounds: ",source," is not in [",
             firstLocalSource_,",",firstLocalSource_+numLocalSources_,")");
        if( numLocalEdges == capacity )
            std::cerr << "WARNING: Pushing back without first reserving space" 
                      << std::endl;
    )
    if( !assembling_ )
        LogicError("Must start assembly before pushing back");
    if( sorted_ && sources_.size() != 0 )
    {
        if( source < sources_.back() )
            sorted_ = false;
        if( source == sources_.back() && target < targets_.back() )
            sorted_ = false;
    }
    sources_.push_back( source );
    targets_.push_back( target );
}

inline void
DistGraph::Empty()
{
    numSources_ = 0;
    numTargets_ = 0;
    SwapClear( sources_ );
    SwapClear( targets_ );
    blocksize_ = 0;
    firstLocalSource_ = 0;
    numLocalSources_ = 0;
    sorted_ = true;
    assembling_ = false;
    SwapClear( localEdgeOffsets_ );
}

inline void
DistGraph::Resize( int numVertices )
{ Resize( numVertices, numVertices ); }

inline void
DistGraph::Resize( int numSources, int numTargets )
{
    const int commRank = mpi::CommRank( comm_ );
    const int commSize = mpi::CommSize( comm_ );
    numSources_ = numSources;
    numTargets_ = numTargets;
    blocksize_ = numSources/commSize;
    firstLocalSource_ = commRank*blocksize_;
    if( commRank < commSize-1 )
        numLocalSources_ = blocksize_;
    else
        numLocalSources_ = numSources - (commSize-1)*blocksize_;
    SwapClear( sources_ );
    SwapClear( targets_ );
    sorted_ = true;
    assembling_ = false;
    SwapClear( localEdgeOffsets_ );
}

inline void
DistGraph::EnsureNotAssembling() const
{
    if( assembling_ )
        LogicError("Should have finished assembling first");
}

inline void
DistGraph::EnsureConsistentSizes() const
{ 
    if( sources_.size() != targets_.size() )
        LogicError("Inconsistent graph sizes");
}

inline void
DistGraph::EnsureConsistentCapacities() const
{ 
    if( sources_.capacity() != targets_.capacity() )
        LogicError("Inconsistent graph capacities");
}

} // namespace cliq

#endif // ifndef CLIQ_CORE_DISTGRAPH_IMPL_HPP
