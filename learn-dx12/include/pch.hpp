#pragma once

#include <cassert>
#include <string>
#include <vector>

#include <Windows.h>
#include <wrl.h>

#include <dxgi.h>
#include <d3d12sdklayers.h>
#include <d3d12.h>
#include <d3dx12.h>

#include <DirectXMath.h>

#include <d3dcompiler.h>

#include <Data\Vertex.hpp>
#include <Data\SceneConstantBuffer.hpp>

#ifndef ThrowIfFailed

#if defined (DEBUG) || (_DEBUG)
#define ThrowIfFailed(result) { assert(SUCCEEDED(result)); }
#else
#define ThrowIfFailed(result) ;
#endif // DEBUG

#endif // ThrowIfFailed