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
    virtual operator Size() const;
    virtual operator Bytes() const;

    Bytes& operator+(Bytes bytes);

protected:
    Size bytesCount_;
};

////////////////////////////////////////
class Kibibytes : public Bytes
{
public:
    Kibibytes(Size bytesCount);
    virtual operator Size() const override;
};

////////////////////////////////////////
class Mibibytes : public Kibibytes
{
public:
    Mibibytes(Size bytesCount);
    virtual operator Size() const override;
};

////////////////////////////////////////
class Gibibytes : public Mibibytes
{
public:
    Gibibytes(Size bytesCount);
    virtual operator Size() const override;
};

}

}