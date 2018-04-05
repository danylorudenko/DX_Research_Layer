#pragma once

#include <pch.hpp>

#include <cstddef>
#include <cinttypes>

namespace DXRL
{

namespace Memory
{

using Byte    = U8;
using BytePtr = U8*;

////////////////////////////////////////
class Bytes
{
public:
    Bytes(Size bytesCount);
    Bytes(Bytes const& bytes);

    operator Size() const;

    Bytes& operator+=(const Bytes& bytes);

protected:
    Size bytesCount_;
};

////////////////////////////////////////
class Kibibytes : public Bytes
{
public:
    Kibibytes(Size bytesCount);
};

////////////////////////////////////////
class Mibibytes : public Kibibytes
{
public:
    Mibibytes(Size bytesCount);
};

////////////////////////////////////////
class Gibibytes : public Mibibytes
{
public:
    Gibibytes(Size bytesCount);
};

Bytes operator+(const Bytes& lhs, const Bytes& rhs);

}

}