#pragma once

#include <cassert>
#include <utility>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stack>

#include <Windows.h>
#include <wrl.h>

#include <dxgi.h>
#include <d3d12sdklayers.h>
#include <d3d12.h>
#include <d3dx12.h>

#include <DirectXMath.h>

#include <d3dcompiler.h>

#define D3D12_RESOURCE_STATE_INVALID static_cast<D3D12_RESOURCE_STATES>(-1)

#define D3D12_GPU_VIRTUAL_ADDRESS_ZERO static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(0)

#ifndef ThrowIfFailed

#if defined (DEBUG) || (_DEBUG)
#define ThrowIfFailed(result) { assert(SUCCEEDED(result)); }
#else
#define ThrowIfFailed(result) result;
#endif // DEBUG

#endif // ThrowIfFailed