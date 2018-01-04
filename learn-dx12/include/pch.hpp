#pragma once

#include <cassert>
#include <utility>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <memory>

#include <Windows.h>
#include <wrl.h>

#include <dxgi.h>
#include <dxgi1_3.h>
#include <d3d12sdklayers.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DXProgrammableCapture.h>

#include <DirectXMath.h>

#include <d3dcompiler.h>

#include <Rendering\Data\Vertex.hpp>
#include <Rendering\Data\SceneConstantBuffer.hpp>

#ifndef ThrowIfFailed

#if defined (DEBUG) || (_DEBUG)
#define ThrowIfFailed(result) { assert(SUCCEEDED(result)); }
#define AssignID3D12ResourceDebugName(name) { resourcePtr_->SetName(name); }
#else
#define ThrowIfFailed(result) result;
#define AssignID3D12ResourceDebugName(name) 0
#endif // DEBUG

#endif // ThrowIfFailed