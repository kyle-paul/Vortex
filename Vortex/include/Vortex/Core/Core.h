#pragma once

// for cross-platform application 
#ifdef VORTEX_EXPORTS
    #define VORTEX_API __declspec(dllexport)
#elif VORTEX_IMPORTS
    #define VORTEX_API __declspec(dllimport)
#else 
    #define VORTEX_API
#endif