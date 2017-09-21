#include <Rendering\GPUCommandQueue.hpp>

GPUCommandQueue::GPUCommandQueue(ID3D12Device* device) :
    fence_(device)
{

}