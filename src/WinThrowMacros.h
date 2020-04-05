#pragma once

#define CHWND_EXCEPT(hr) Window::Exception ex = Window::Exception(__LINE__, __FILE__, hr); DEBUG_BREAK(); OutputDebugString(ex.what()); throw ex
#define CHWND_THROW_LAST_EXCEPT() Window::Exception ex = Window::Exception(__LINE__, __FILE__, GetLastError()); OutputDebugString(ex.what()); DEBUG_BREAK(); throw ex
#define CHWND_NOGFX_EXCEPTION() Window::NoGfxException ex = Window::NoGfxException(__LINE__, __FILE__); DEBUG_BREAK(); OutputDebugString(ex.what()); throw ex