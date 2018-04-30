#include <pch.hpp>
#include <chrono>
#include <random>
#include <bitset>

#include <Memory\Allocator.hpp>
#include <Memory\Pointer.hpp>

struct CharBuffer40
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

    Memory::Mibibytes chunkSize{ 10 };
    void* chunk = malloc(chunkSize);

    Memory::FreeListAllocator allocator{ chunk, chunkSize };

    Size constexpr allocationsCount = 100;
    CharBuffer40* allocations[allocationsCount];
    
    for (Size i = 0; i < 3; ++i) {
        CharBuffer40* buffer = allocator.Alloc<CharBuffer40>();
        *(allocations + i) = buffer;
        strcpy_s(buffer->content, sizeof(CharBuffer40), testString);
    }
    
    allocator.Free(*(allocations + 1));

    for (Size i = 3; i < 7; ++i) {
        CharBuffer40* buffer = allocator.Alloc<CharBuffer40>();
        *(allocations + i) = buffer;
        strcpy_s(buffer->content, sizeof(CharBuffer40), testString);
    }

    allocator.Free(*(allocations + 5));
    allocator.Free(*(allocations + 6));

    CharBuffer40* buffer = allocator.Alloc<CharBuffer40>();
    strcpy_s(buffer->content, sizeof(CharBuffer40), testString);
    
    free(chunk);
    
    system("pause");
    return 0;
}