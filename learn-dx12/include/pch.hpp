#pragma once

#include <cassert>
#include <string>

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
#define ThrowIfFailed(result) { HRESULT hr__ = (result); if(FAILED(hr__)) { std::string msg = "HRESULT failed in file: "; msg += __FILE__; msg += ", line: "; msg += std::to_string(__LINE__); throw std::exception(msg.c_str()); } }
#endif