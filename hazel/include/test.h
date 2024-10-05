#pragma once

#ifdef _WIN32
    #ifdef HAZEL_EXPORTS
        #define HAZEL_API __declspec(dllexport)
    #else
        #define HAZEL_API __declspec(dllimport)
    #endif
#else
    #define HAZEL_API
#endif

namespace Hazel {
	HAZEL_API void Print();
}