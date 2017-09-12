#pragma once

#include <cassert>

#include <wrl.h>

#include <d3d12sdklayers.h>
#include <d3d12.h>

#define ThrowIfFailed(result) if(FAILED(result)) { std::string msg = "HRESULT failed in file: "; msg += __FILE__; msg += ", line: "; msg += __LINE__; throw std::exception(msg.c_str()); }