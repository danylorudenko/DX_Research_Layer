#include <pch.hpp>
#include <chrono>
#include <random>
#include <bitset>

#include <Memory\Allocator.hpp>
#include <Memory\Pointer.hpp>

struct CharBuffer32
{
    union
    {
        //DXRL::U64 contentInt[5];
        char content[40];
    };
};

int main()
{
    using namespace DXRL;
    
    char constexpr* testString{ "012345678901234567890123456789" };

    ///////////////////////////////////////////////////

    Memory::PoolAllocator<U64> pool{ nullptr, 0 };

    //Size constexpr allocationsCount = 10;
    //CharBuffer32* allocations[allocationsCount];
    //
    //
    //Memory::Mibibytes linearChunkSize{ 10 };
    //Memory::VoidPtr linearMemoryChunk = malloc(linearChunkSize);
    //Memory::StackAllocator allocator{ linearMemoryChunk, linearChunkSize, true };
    //
    //for (Size i = 0; i < allocationsCount; ++i) {
    //    CharBuffer32* buffer = allocator.Alloc<CharBuffer32>();
    //    strcpy_s(buffer->content, sizeof(CharBuffer32), testString);
    //    *(allocations + i) = buffer;
    //
    //}
    //
    //for (Size i = 9; i >= 7; --i) {
    //    allocator.Free(*(allocations + i));
    //}
    //
    //CharBuffer32* buffer = allocator.Alloc<CharBuffer32>();
    //strcpy_s(buffer->content, sizeof(CharBuffer32), testString);
    
    system("pause");
    return 0;
}