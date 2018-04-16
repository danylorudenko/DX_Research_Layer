#include <pch.hpp>

#include <Memory\Allocator.hpp>

struct TestStruct
{
    char content[512];
};

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PSTR pCmdLine, _In_ int nShowCmd)
{
    using namespace DXRL;

    std::string const testString{ "THIS IS A TEST STRING!" };

    Memory::Mibibytes const memorySize{ 1 };
    Memory::VoidPtr memory = malloc(memorySize);

    Memory::LinearAllocator allocator{ memory, memorySize };

    auto* charBuffer0 = allocator.Alloc<TestStruct>();
    auto* charBuffer1 = allocator.Alloc<TestStruct>();
    auto* charBuffer2 = allocator.Alloc<TestStruct>();
    
    strcpy_s(charBuffer0->content, sizeof(TestStruct), testString.c_str());
    strcpy_s(charBuffer1->content, sizeof(TestStruct), testString.c_str());
    strcpy_s(charBuffer2->content, sizeof(TestStruct), testString.c_str());

    allocator.FreeAll();

    free(memory);

    return 0;
}