#include <iostream>
#include <map>
#include <cinttypes>

class AllocatorLinearNode
{
public:
    void* nodeMemory_;
    std::size_t size_;
    std::size_t free_;
};

class MiniAllocator
{
public:
    MiniAllocator(char* buffer, std::size_t bytes)
    {
        buffer_ = buffer;
        freeRegions_[0] = bytes;
    }

    char* Alloc(std::size_t bytes)
    {
        
    }

    void Free(char* memory)
    {
        

    }

    char* buffer_;
    std::map<char*, std::size_t> freeRegions_;
    std::map<char*, std::size_t> busyRegions_;
};


int main()
{
    char* memory = new char[1024 * 1024];


    system("pause");
    return 0;
}