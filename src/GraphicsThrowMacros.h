#pragma once
#include "Core.h"

#define GFX_THROW_FAILED(_hr) if( FAILED(_hr)){ auto ex = Graphics::HrException(__LINE__, __FILE__, _hr); OutputDebugString(ex.what()); DEBUG_BREAK(); throw ex;}
#define GFX_DEVICE_REMOVED_EXCEPT(_hr)  auto ex = Graphics::DeviceRemovedException(__LINE__, __FILE__, _hr); OutputDebugString(ex.what()); DEBUG_BREAK(); throw ex