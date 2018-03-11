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
#include <algorithm>

#include <Windows.h>
#include <windowsx.h>
#include <wrl.h>

#include <dxgi.h>
#include <dxgi1_3.h>
#include <d3d12sdklayers.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DXProgrammableCapture.h>

#include <DirectXMath.h>

#include <d3dcompiler.h>

#ifndef ThrowIfFailed

#if defined (DEBUG) || (_DEBUG)
#define ThrowIfFailed(result) { assert(SUCCEEDED(result)); }
#define AssignID3D12ResourceDebugName(name) { resourcePtr_->SetName(name); }
#else
#define ThrowIfFailed(result) result;
#define AssignID3D12ResourceDebugName(name) 0
#endif // DEBUG

#endif // ThrowIfFailed

float constexpr DEGREE_TO_RAD = DirectX::XM_PI / 180.0f;
float constexpr RAD_TO_DEGREE = 180.0f / DirectX::XM_PI;