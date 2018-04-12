#pragma once

#include <Foundation\Macro.hpp>
#include <Memory\Bytes.hpp>

namespace DXRL
{

namespace Memory
{

////////////////////////////////////////
class LinearAllocator
{
public:
    DXRL_DEFINE_UNCOPYABLE_MOVABLE(LinearAllocator)

    LinearAllocator(VoidPtr chunk, Size size, bool isOwner = false);
    
    ~LinearAllocator();

    VoidPtr Alloc(Size size, Size alignment = 0);
    
    void Reset();

private:

    VoidPtr mainChunk_;
    Size mainChunkSize_;
    bool isOwner_;

    Size allocated_;
};

} // namespace Memory

} // namespace DXRL