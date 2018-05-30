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
    Bytes(Size_t bytesCount);
    Bytes(Bytes const& bytes);

    operator Size_t() const;

    Bytes& operator+=(const Bytes& rhs);
    Bytes& operator-=(const Bytes& rhs);
    Bytes& operator*=(Size_t rhs);

protected:
    Size_t bytesCount_;
};

////////////////////////////////////////
class Kibibytes : public Bytes
{
public:
    Kibibytes(Size_t kibiBytesCount);
};

////////////////////////////////////////
class Mibibytes : public Kibibytes
{
public:
    Mibibytes(Size_t mibiBytesCount);
};

////////////////////////////////////////
class Gibibytes : public Mibibytes
{
public:
    Gibibytes(Size_t gibiBytesCount);
};

Bytes operator+(const Bytes& lhs, const Bytes& rhs);
Bytes operator-(const Bytes& lhs, const Bytes& rhs);
Bytes operator*(const Bytes& lhs, Size_t rhs);

} // namespace Memory

} // namespace DXRL