#include <pch.hpp>
#include <chrono>
#include <random>

#include <Memory\Allocator.hpp>
#include <Memory\Pointer.hpp>

struct CharBuffer32
{
    char content[32];
};

int main()
{
    using namespace DXRL;
    
    char* const testString{ "012345678901234567890123456789" };

    ///////////////////////////////////////////////////

    Size constexpr allocationsCount = 10;
    //CharBuffer32* bufferPtrArray[allocationsCount];

    Memory::Bytes linearChunkSize{ sizeof(CharBuffer32) * allocationsCount };
    Memory::VoidPtr linearMemoryChunk = malloc(linearChunkSize);
    Memory::LinearAllocator allocator{ linearMemoryChunk, linearChunkSize, true };

    for (Size i = 0; i < allocationsCount; ++i) {
        CharBuffer32* buffer = allocator.Alloc<CharBuffer32>();
        strcpy_s(buffer->content, sizeof(CharBuffer32), testString);
    }
    
    system("pause");
    return 0;
}