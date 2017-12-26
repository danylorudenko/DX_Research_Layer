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
#include <dxgi1_3.h>
#include <d3d12sdklayers.h>
#include <d3d12.h>
#include <d3dx12.h>

#include <DirectXMath.h>

#include <d3dcompiler.h>

#include <Rendering\Data\Vertex.hpp>
#include <Rendering\Data\SceneConstantBuffer.hpp>

#ifndef ThrowIfFailed

#if defined (DEBUG) || (_DEBUG)
#define ThrowIfFailed(result) { assert(SUCCEEDED(result)); }
#else
#define ThrowIfFailed(result) result;
#endif // DEBUG

#endif // ThrowIfFailed