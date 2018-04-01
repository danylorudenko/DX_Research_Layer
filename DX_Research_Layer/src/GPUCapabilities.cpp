#include <Rendering\Misc\GPUCapabilities.hpp>

#include <Rendering\GPUFoundation.hpp>

namespace DXRL
{

std::wstring GPUCapabilities::ListAdapters(GPUFoundation& gpuAccess)
{
    std::wstringstream output;
    auto& factory = gpuAccess.DXGIFactory();

    Microsoft::WRL::ComPtr<IDXGIAdapter1> adapterTemp{ nullptr };
    for (UINT i = 0; factory->EnumAdapters1(i, adapterTemp.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; i++) {
        DXGI_ADAPTER_DESC1 adapterDesc{};
        {
            output << "Adapter " << i << ":" << std::endl;

            {
                auto const result = D3D12CreateDevice(adapterTemp.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr);
                output << "D3D12_FATURE_LEVEL_11_0 support: " << (SUCCEEDED(result) ? "TRUE" : "FALSE") << std::endl;
            }


            auto const result = adapterTemp->GetDesc1(&adapterDesc);

            if (!SUCCEEDED(result)) {
                output << "Failed getting adapter description." << std::endl;
                continue;
            }

            output
                << "Description: " << adapterDesc.Description << std::endl
                << "LUID: " << adapterDesc.AdapterLuid.HighPart << "/" << adapterDesc.AdapterLuid.LowPart << std::endl
                << "Software adapter: " << (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE ? "TRUE" : "FALSE") << std::endl
                << "System Memory: " << adapterDesc.DedicatedSystemMemory << std::endl
                << "Video Memory: " << adapterDesc.DedicatedVideoMemory << std::endl << std::endl;
        }

    }

    return output.str();
}

Microsoft::WRL::ComPtr<ID3D12Device> GPUCapabilities::GenerateStandardDeviceQuery(GPUFoundation& gpuAccess)
{
    Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter{ nullptr };
    auto& factory = gpuAccess.DXGIFactory();

    OutputDebugStringA("Attempting to select D3D12 device.\n");

    for (UINT i = 0; factory->EnumAdapters1(i, adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; i++) {
        DXGI_ADAPTER_DESC1 adapterDesc;

        auto const result = adapter->GetDesc1(&adapterDesc);
        if (SUCCEEDED(result)) {
            if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
                continue;
            }

            Microsoft::WRL::ComPtr<ID3D12Device> device{ nullptr };
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(device.GetAddressOf())))) {
                std::wstringstream output{};
                output
                    << "Description: " << adapterDesc.Description << std::endl
                    << "LUID: " << adapterDesc.AdapterLuid.HighPart << "/" << adapterDesc.AdapterLuid.LowPart << std::endl
                    << "Software adapter: " << (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE ? "TRUE" : "FALSE") << std::endl
                    << "System Memory: " << adapterDesc.DedicatedSystemMemory << std::endl
                    << "Video Memory: " << adapterDesc.DedicatedVideoMemory << std::endl << std::endl;
                OutputDebugStringW(output.str().c_str());

                return device;
            }
        }
    }

    char constexpr* criticalErrorMessage = "CRITICAL FAILURE: couldn't find valid adapter to create ID3D12Device.";
    OutputDebugStringA(criticalErrorMessage);

    return Microsoft::WRL::ComPtr<ID3D12Device>{ nullptr };

}

}