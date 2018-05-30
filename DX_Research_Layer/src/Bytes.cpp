#include <pch.hpp>

namespace DXRL
{

namespace Memory
{

////////////////////////////////////////
Bytes::Bytes(Size_t bytesCount)
    : bytesCount_{ bytesCount }
{ }

Bytes::Bytes(Bytes const& bytes)
    : bytesCount_{ bytes.bytesCount_ }
{ }

Bytes::operator Size_t() const
{
    return bytesCount_;
}

Bytes& Bytes::operator+=(const Bytes& rhs)
{
    bytesCount_ += rhs.bytesCount_;
    return *this;
}

Bytes& Bytes::operator-=(const Bytes& rhs)
{
    bytesCount_ -= rhs.bytesCount_;
    return *this;
}

Bytes& Bytes::operator*=(Size_t rhs)
{
    bytesCount_ *= rhs;
    return *this;
}


////////////////////////////////////////
Kibibytes::Kibibytes(Size_t kibibytesCount) :
    Bytes{ kibibytesCount * 1024 }
{ }


////////////////////////////////////////
Mibibytes::Mibibytes(Size_t mibibytesCount) :
    Kibibytes{ mibibytesCount * 1024 }
{ }


////////////////////////////////////////
Gibibytes::Gibibytes(Size_t gibibytesCount)
    : Mibibytes{ gibibytesCount * 1024 }
{ }


////////////////////////////////////////
Bytes operator+(const Bytes& lhs, const Bytes& rhs)
{
    return Bytes{ (Size_t)lhs + (Size_t)rhs };
}

Bytes operator-(const Bytes& lhs, const Bytes& rhs)
{
    return Bytes{ (Size_t)lhs - (Size_t)rhs };
}

Bytes operator*(const Bytes& lhs, Size_t rhs)
{
    return Bytes{ (Size_t)lhs * rhs };
}

} // namespace Memory

} // namespace DXRL
