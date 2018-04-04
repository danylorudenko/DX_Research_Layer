#pragma once

#include <cassert>
#include <cstddef>
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
#include <3rdParty/d3dx12.h>
#include <DXProgrammableCapture.h>

#include <DirectXMath.h>

#include <d3dcompiler.h>

////////////////////////////////////////

namespace DXRL
{

using S8   = std::int8_t;
using S16  = std::int16_t;
using S32  = std::int32_t;
using S64  = std::int64_t;

using U8   = std::uint8_t;
using U16  = std::uint16_t;
using U32  = std::uint32_t;
using U64  = std::uint64_t;

using Size = std::size_t;

using Single = float;
using Double = double;

}

////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////

#ifndef DXRL_THROW_IF_FAILED

#if defined (DEBUG) || (_DEBUG)
#define DXRL_THROW_IF_FAILED(result) { assert(SUCCEEDED(result)); }
#else
#define DXRL_THROW_IF_FAILED(result) result;
#endif // DEBUG

#endif // DXRL_THROW_IF_FAILED

////////////////////////////////////////

#ifndef DXRL_DEFINE_UNCOPYABLE_MOVABLE_DEFAULT

#define DXRL_DEFINE_UNCOPYABLE_MOVABLE_DEFAULT(className)                       \
                                                                                \
    className() = default;                                                      \
    className(className const&) = delete;                                       \
    className& operator=(className const&) = delete;                            \
                                                                                \
    className(className&&) = default;                                           \
    className& operator=(className&&) = default;                                \
                                                                                \

#endif // DXRL_DEFINE_UNCOPYABLE_MOVABLE_DEFAULT

////////////////////////////////////////

#ifndef DXRL_DEFINE_UNCOPYABLE_MOVABLE

#define DXRL_DEFINE_UNCOPYABLE_MOVABLE(className)                               \
                                                                                \
    className() = default;                                                      \
    className(className const&) = delete;                                       \
    className& operator=(className const&) = delete;                            \
                                                                                \
    className(className&&);                                                     \
    className& operator=(className&&);                                          \


#endif // DXRL_DEFINE_UNCOPYABLE_MOVABLE

////////////////////////////////////////

#ifndef DXRL_DEFINE_COPYABLE_DEFAULT_MOVABLE_DEFAULT

#define DXRL_DEFINE_COPYABLE_DEFAULT_MOVABLE_DEFAULT(className)                 \
                                                                                \
    className() = default;                                                      \
    className(className const&) = default;                                      \
    className& operator=(className const&) = default;                           \
                                                                                \
    className(className&&);                                                     \
    className& operator=(className&&);                                          \
                                                                                \

#endif // DXRL_DEFINE_COPYABLE_DEFAULT_MOVABLE_DEFAULT

////////////////////////////////////////

#ifndef DXRL_DEFINE_COPYABLE_DEFAULT_MOVABLE

#define DXRL_DEFINE_COPYABLE_DEFAULT_MOVABLE(className)                         \
                                                                                \
    className() = default;                                                      \
    className(className const&) = default;                                      \
    className& operator=(className const&) = default;                           \
                                                                                \
    className(className&&);                                                     \
    className& operator=(className&&);                                          \

#endif // DXRL_DEFINE_COPYABLE_DEFAULT_MOVABLE

////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////

float constexpr DEGREE_TO_RAD = DirectX::XM_PI / 180.0f;
float constexpr RAD_TO_DEGREE = 180.0f / DirectX::XM_PI;