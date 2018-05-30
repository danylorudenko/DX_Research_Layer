#pragma once

#include <pch.hpp>

namespace DXRL
{

using S8_t  = std::int8_t;
using S16_t = std::int16_t;
using S32_t = std::int32_t;
using S64_t = std::int64_t;

using U8_t  = std::uint8_t;
using U16_t = std::uint16_t;
using U32_t = std::uint32_t;
using U64_t = std::uint64_t;

using S32_fast_t = std::int_fast32_t;
using S64_fast_t = std::int_fast64_t;

using U32_fast_t = std::uint_fast32_t;
using U64_fast_t = std::uint_fast64_t;


using Size_t = std::size_t;

using Byte = U8_t;

using Single = float;
using Double = double;

using String = std::string;
using WideString = std::wstring;


namespace Memory
{

using VoidPtr = void*;
using BytePtr = U8_t*;
using UintPtr = std::uintptr_t;
using PtrDiff = std::ptrdiff_t;

template<typename T>
using UniquePtr = std::unique_ptr<T>;

} // namespace Memory

} // namespace DXRL