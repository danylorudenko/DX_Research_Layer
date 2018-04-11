#pragma once

#include <cstddef>
#include <cinttypes>

#include <Foundation\Types.hpp>

namespace DXRL
{

namespace Memory
{

////////////////////////////////////////
class Bytes
{
public:
    Bytes(Size bytesCount);
    Bytes(Bytes const& bytes);

    operator Size() const;

    Bytes& operator+=(const Bytes& rhs);
    Bytes& operator-=(const Bytes& rhs);
    Bytes& operator*=(Size rhs);

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
Bytes operator-(const Bytes& lhs, const Bytes& rhs);
Bytes operator*(const Bytes& lhs, Size rhs);

} // namespace Memory

} // namespace DXRL