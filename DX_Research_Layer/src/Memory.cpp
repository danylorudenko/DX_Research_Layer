#include <Memory\Memory.hpp>

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

Bytes::operator Bytes() const
{
    return Bytes{ bytesCount_ };
}

Bytes& Bytes::operator+(Bytes bytes)
{
    bytesCount_ += bytes.bytesCount_;
    return *this;
}

////////////////////////////////////////
Kibibytes::Kibibytes(Size kibibytesCount) :
    Bytes{ kibibytesCount * 1024 }
{ }

Kibibytes::operator Size() const
{
    return bytesCount_ * 1024;
}

////////////////////////////////////////
Mibibytes::Mibibytes(Size mibibytesCount) :
    Kibibytes{ mibibytesCount * 1024 }
{ }

Mibibytes::operator Size() const
{
    return bytesCount_ * (1024 * 1024);
}

////////////////////////////////////////
Gibibytes::Gibibytes(Size bytesCount)
    : Mibibytes{ bytesCount * 1024 }
{ }

Gibibytes::operator Size() const
{
    return bytesCount_ * (1024 * 1024 * 1024);
}

}

}
