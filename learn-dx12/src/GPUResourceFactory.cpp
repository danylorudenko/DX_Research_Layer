#include <Rendering\Data\Resource\GPUResourceFactory.hpp>

GPUResourceFactory::GPUResourceFactory() = default;

GPUResourceFactory::GPUResourceFactory(GPUFoundation const& foundation, std::size_t framebufferCount) :
    staticResourcesAllocator_{ foundation }, staticResourcesTable_{ framebufferCount }
{ }

GPUResourceFactory::GPUResourceFactory(GPUResourceFactory&& rhs) = default;

GPUResourceFactory& GPUResourceFactory::operator=(GPUResourceFactory&& rhs) = default;

GPUResourceID GPUResourceFactory::AllocStaticResource(std::size_t framesCount, D3D12_RESOURCE_DESC const& desc, D3D12_RESOURCE_STATES initialState)
{
    GPUResourceDirectID ids[5];
    assert(framesCount <= 5 && "Maximum frame limit per logical resource is 5!!!");

    for (size_t i = 0; i < framesCount; i++) {
        ids[i] = staticResourcesAllocator_.Alloc(desc, initialState);
    }

    return staticResourcesTable_.InsertResource(framesCount, ids);
}

GPUResource& GPUResourceFactory::AccessStaticResource(std::size_t frameIndex, GPUResourceID id)
{
    auto const directID = staticResourcesTable_.FetchDirectID(frameIndex, id);
    return staticResourcesAllocator_.AccessResource(directID);
}

GPUResource& GPUResourceFactory::AccessFramebuffer(std::size_t frameIndex)
{
    return staticResourcesAllocator_.AccessFramebuffer(frameIndex);
}