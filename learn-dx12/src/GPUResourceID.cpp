#include <Rendering\Data\Resource\GPUResourceID.hpp>

GPUResourceDirectID::GPUResourceDirectID() = default;

GPUResourceDirectID::GPUResourceDirectID(std::size_t ID) :
    ID_{ ID }
{ }

GPUResourceDirectID::GPUResourceDirectID(GPUResourceDirectID const&) = default;

GPUResourceDirectID::GPUResourceDirectID(GPUResourceDirectID&&) = default;

GPUResourceDirectID& GPUResourceDirectID::operator=(GPUResourceDirectID const&) = default;

GPUResourceDirectID& GPUResourceDirectID::operator=(GPUResourceDirectID&&) = default;

/////////////////////////////////////////////////////////////////////////////////////////

GPUResourceID::GPUResourceID() = default;

GPUResourceID::GPUResourceID(std::size_t ID) :
    ID_{ ID }
{ }

GPUResourceID::GPUResourceID(GPUResourceID const& rhs) = default;

GPUResourceID::GPUResourceID(GPUResourceID&& rhs) = default;

GPUResourceID& GPUResourceID::operator=(GPUResourceID const& rhs) = default;

GPUResourceID& GPUResourceID::operator=(GPUResourceID&& rhs) = default;

///////////////////////////////////////////////////////////////////////////////////////////