#pragma once
#include <memory>

// for cross-platform application 
#ifdef VORTEX_EXPORTS
    #define VORTEX_API __declspec(dllexport)
#elif VORTEX_IMPORTS
    #define VORTEX_API __declspec(dllimport)
#else 
    #define VORTEX_API
#endif

#define VX_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Vortex 
{
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T>
    using Ref = std::shared_ptr<T>;
    
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

#include "Vortex/Debug/Instrumentor.h"
#include "Vortex/Core/Assert.h"