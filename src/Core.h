#pragma once

#ifdef NDEBUG
#define DEBUG_BREAK()
#else
#define DEBUG_BREAK() __debugbreak()
#endif // !NDEBUG