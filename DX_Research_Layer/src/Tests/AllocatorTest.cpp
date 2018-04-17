#include <pch.hpp>
#include <chrono>

#include <Memory\Allocator.hpp>

struct CharBuffer32
{
    char content[32];
};

struct CharBuffer256
{
    char content[256];
};

struct CharBuffer1024
{
    char content[1024];
};

int main()
{
    using namespace DXRL;

    std::string const testString{ "THIS IS A TEST STRING!" };
    Size constexpr allocationsCount = 1024;

    ///////////////////////////////////////////////////

    Memory::VoidPtr* linearAllocations = new Memory::VoidPtr[allocationsCount];
    Memory::VoidPtr* newAllocations = new Memory::VoidPtr[allocationsCount];


    Memory::Bytes linearChunkSize{ sizeof(CharBuffer32) * allocationsCount * 1024 };
    Memory::VoidPtr linearMemoryChunk = malloc(linearChunkSize);
    Memory::LinearAllocator allocator{ linearMemoryChunk, linearChunkSize };

    //////////////////////////////////////
    // LINEAR TEST START
    auto linearStart = std::chrono::system_clock::now();

    for (Size i = 0; i < allocationsCount; ++i) {
        Size const id = i % 3;
        switch (id)
        {
        case 0:
            *(linearAllocations + i) = allocator.Alloc(sizeof(CharBuffer32));
            break;
        case 1:
            *(linearAllocations + i) = allocator.Alloc(sizeof(CharBuffer256));
            break;
        case 2:
            *(linearAllocations + i) = allocator.Alloc(sizeof(CharBuffer1024));
            break;
        }
        
        //strcpy_s((*linearAllocations + i)->content, sizeof(CharBuffer32), testString.c_str());
    }

    allocator.FreeAll();
    free(linearMemoryChunk);

    auto linearEnd = std::chrono::system_clock::now();
    // LINEAR TEST END
    //////////////////////////////////////
    auto linearDuration = linearEnd - linearStart;
    std::cout << allocationsCount << " linear allocations: " << linearDuration.count() << " nanoseconds" << std::endl << std::endl;




    //////////////////////////////////////
    // NEW TEST START
    auto newStart = std::chrono::system_clock::now();

    for (Size i = 0; i < allocationsCount; ++i) {
        for (Size i = 0; i < allocationsCount; ++i) {
            Size const id = i % 3;
            switch (id)
            {
            case 0:
                *(linearAllocations + i) = malloc(sizeof(CharBuffer32));
                break;
            case 1:
                *(linearAllocations + i) = malloc(sizeof(CharBuffer256));
                break;
            case 2:
                *(linearAllocations + i) = malloc(sizeof(CharBuffer1024));
                break;
            }

            //strcpy_s((*linearAllocations + i)->content, sizeof(CharBuffer32), testString.c_str());
        }
        //strcpy_s((*(newAllocations + i))->content, sizeof(CharBuffer32), testString.c_str());
        free(*(newAllocations + i));
    }

    //for (Size i = 0; i < allocationsCount; ++i) {
    //    
    //}

    auto newEnd = std::chrono::system_clock::now();
    // NEW TEST END
    //////////////////////////////////////

    auto newDuration = newEnd - newStart;
    std::cout << allocationsCount << " new allocations: " << newDuration.count() << " nanoseconds" << std::endl << std::endl;

    ////////////////////////////////////////////////////

    delete[] linearAllocations;
    delete[] newAllocations;

    system("pause");
    return 0;
}