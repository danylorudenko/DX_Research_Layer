#include <Rendering\Data\GPURenderItem.hpp>

namespace DXRL
{
	
GPURenderItem::GPURenderItem() = default;

GPURenderItem::GPURenderItem(GPURenderItem&& rhs) = default;

GPURenderItem& GPURenderItem::operator=(GPURenderItem&& rhs) = default;

void GPURenderItem::CreateTransformBuffer(std::size_t frames, std::size_t bindSlot, GPUFoundation& foundation)
{
	D3D12_RESOURCE_DESC rd{};
	rd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	rd.Width = 256;
	rd.Height = 1;
	rd.DepthOrArraySize = 1;
	rd.Alignment = 0;
	rd.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	rd.Format = DXGI_FORMAT_UNKNOWN;
	rd.MipLevels = 1;
	rd.SampleDesc.Count = 1;
	rd.SampleDesc.Quality = 0;
	rd.Flags = D3D12_RESOURCE_FLAG_NONE;
	
	std::vector<GPUResourceHandle> resHandles{};
	resHandles.resize(frames);
	for (std::size_t i = 0; i < frames; i++) {
		resHandles[i] = foundation.AllocUploadResource(rd, D3D12_RESOURCE_STATE_GENERIC_READ);
	}

	std::vector<D3D12_CONSTANT_BUFFER_VIEW_DESC> cbvd;
	cbvd.resize(frames);
	for (std::size_t i = 0; i < frames; i++) {
		cbvd[i].BufferLocation = resHandles[i].Resource().GetPtr()->GetGPUVirtualAddress();
		cbvd[i].SizeInBytes = 256;
	}

	GPUResourceViewHandle cbvHandle = foundation.AllocCBV(frames, resHandles.data(), cbvd.data(), D3D12_RESOURCE_STATE_GENERIC_READ);

	dynamicArg_ = GPUDynamicRootArgument{ bindSlot, GPUResourceViewTable{1, &cbvHandle } };
}

}
