#include <pch.hpp>
#include <chrono>
#include <random>

#include <Memory\Allocator.hpp>
#include <Memory\Pointer.hpp>

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

    std::random_device device{};
    std::default_random_engine engine{ device() };

    std::uniform_int_distribution<> uniform_dist{ 100, 10000 };
    
    std::string const testString{ "THIS IS A TEST STRING!" };
    Size constexpr allocationsCount = 10000;

    ///////////////////////////////////////////////////

    Memory::VoidPtr* linearAllocations = new Memory::VoidPtr[allocationsCount];
    Memory::VoidPtr* newAllocations = new Memory::VoidPtr[allocationsCount];


    Memory::Bytes linearChunkSize{ allocationsCount * 10000 };
    Memory::VoidPtr linearMemoryChunk = malloc(linearChunkSize);
    Memory::LinearAllocator allocator{ linearMemoryChunk, linearChunkSize, true };

    //////////////////////////////////////
    // LINEAR TEST START
    auto linearStart = std::chrono::high_resolution_clock::now();;

    for (Size i = 0; i < allocationsCount; ++i) {
        char* ptr = reinterpret_cast<char*>(allocator.Alloc(uniform_dist(engine)));
        strcpy_s(ptr, 80, testString.c_str());
    }

    allocator.Reset();

    auto linearEnd = std::chrono::high_resolution_clock::now();
    
    // LINEAR TEST END
    //////////////////////////////////////
    auto linearDuration = linearEnd - linearStart;
    std::cout << allocationsCount << " linear allocations: " << linearDuration.count() << " nanoseconds" << std::endl << std::endl;




    //////////////////////////////////////
    // NEW TEST START
    auto newStart = std::chrono::high_resolution_clock::now();;

    for (Size i = 0; i < allocationsCount; ++i) {
        char* ptr = reinterpret_cast<char*>(malloc(uniform_dist(engine)));
        *(newAllocations + i) = ptr;
        strcpy_s(ptr, 80, testString.c_str());
    }

    for (Size i = 0; i < allocationsCount; ++i) {
        free(*(newAllocations + i));
    }

    auto newEnd = std::chrono::high_resolution_clock::now();;
    // NEW TEST END
    //////////////////////////////////////

    auto newDuration = newEnd - newStart;
    std::cout << allocationsCount << " ___new allocations: " << newDuration.count() << " nanoseconds" << std::endl << std::endl;

    ////////////////////////////////////////////////////

    delete[] linearAllocations;
    delete[] newAllocations;

    system("pause");
    return 0;
}