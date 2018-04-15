#pragma once

#include <pch.hpp>

namespace DXRL
{

using S8  = std::int8_t;
using S16 = std::int16_t;
using S32 = std::int32_t;
using S64 = std::int64_t;

using U8  = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;

using Size = std::size_t;

using Byte = U8;

using Single = float;
using Double = double;

using String = std::string;
using WideString = std::wstring;


namespace Memory
{

using VoidPtr = void*;
using BytePtr = U8*;
using UintPtr = std::uintptr_t;
using PtrDiff = std::ptrdiff_t;

template<typename T>
using UniquePtr = std::unique_ptr<T>;

} // namespace Memory

} // namespace DXRL