#include <pch.hpp>

namespace DXRL
{

namespace Memory
{

////////////////////////////////////////
Bytes::Bytes(Size bytesCount)
    : bytesCount_{ bytesCount }
{ }

Bytes::Bytes(Bytes const& bytes)
    : bytesCount_{ bytes.bytesCount_ }
{ }

Bytes::operator Size() const
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

Bytes& Bytes::operator*=(Size rhs)
{
    bytesCount_ *= rhs;
    return *this;
}


////////////////////////////////////////
Kibibytes::Kibibytes(Size kibibytesCount) :
    Bytes{ kibibytesCount * 1024 }
{ }


////////////////////////////////////////
Mibibytes::Mibibytes(Size mibibytesCount) :
    Kibibytes{ mibibytesCount * 1024 }
{ }


////////////////////////////////////////
Gibibytes::Gibibytes(Size bytesCount)
    : Mibibytes{ bytesCount * 1024 }
{ }


////////////////////////////////////////
Bytes operator+(const Bytes& lhs, const Bytes& rhs)
{
    return Bytes{ (Size)lhs + (Size)rhs };
}

Bytes operator-(const Bytes& lhs, const Bytes& rhs)
{
    return Bytes{ (Size)lhs - (Size)rhs };
}

Bytes operator*(const Bytes& lhs, Size rhs)
{
    return Bytes{ (Size)lhs * rhs };
}

} // namespace Memory

} // namespace DXRL
