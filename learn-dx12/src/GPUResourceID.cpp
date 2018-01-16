#include <Rendering\Data\Resource\GPUResourceID.hpp>

GPUResourceDirectID::GPUResourceDirectID() = default;

GPUResourceDirectID::GPUResourceDirectID(std::size_t ID) :
    ID_{ ID }
{ }

GPUResourceDirectID::GPUResourceDirectID(GPUResourceDirectID const&) = default;

GPUResourceDirectID::GPUResourceDirectID(GPUResourceDirectID&&) = default;

GPUResourceDirectID& GPUResourceDirectID::operator=(GPUResourceDirectID const&) = default;

GPUResourceDirectID& GPUResourceDirectID::operator=(GPUResourceDirectID&&) = default;

std::size_t GPUResourceDirectID::ID() const { return ID_; }

/////////////////////////////////////////////////////////////////////////////////////////

GPUResourceID::GPUResourceID() = default;

GPUResourceID::GPUResourceID(std::size_t ID) :
    ID_{ ID }
{ }

GPUResourceID::GPUResourceID(GPUResourceID const& rhs) = default;

GPUResourceID::GPUResourceID(GPUResourceID&& rhs) = default;

GPUResourceID& GPUResourceID::operator=(GPUResourceID const& rhs) = default;

GPUResourceID& GPUResourceID::operator=(GPUResourceID&& rhs) = default;

std::size_t GPUResourceID::ID() const { return ID_; }

///////////////////////////////////////////////////////////////////////////////////////////

GPUResourceViewDirectID::GPUResourceViewDirectID() = default;

GPUResourceViewDirectID::GPUResourceViewDirectID(std::size_t ID) :
    ID_{ ID }
{ }

GPUResourceViewDirectID::GPUResourceViewDirectID(GPUResourceViewDirectID const& rhs) = default;

GPUResourceViewDirectID::GPUResourceViewDirectID(GPUResourceViewDirectID&& rhs) = default;

GPUResourceViewDirectID& GPUResourceViewDirectID::operator=(GPUResourceViewDirectID const& rhs) = default;

GPUResourceViewDirectID& GPUResourceViewDirectID::operator=(GPUResourceViewDirectID&& rhs) = default;

std::size_t GPUResourceViewDirectID::ID() const { return ID_; }