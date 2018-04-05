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

Bytes& Bytes::operator+=(const Bytes& bytes)
{
    bytesCount_ += bytes.bytesCount_;
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

Bytes operator+(const Bytes& lhs, const Bytes& rhs)
{
    return Bytes{ (Size)lhs + (Size)rhs };
}

}

}
