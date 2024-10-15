#pragma once


#include "Vortex/Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include "spdlog/sinks/stdout_color_sinks.h"

#include <memory>


namespace Vortex 
{
    class VORTEX_API Logging 
    {
    public:
        static void Init();
        static inline std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }
        static inline std::shared_ptr<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}


// Core logging macros
#define VX_CORE_TRACE(...)    ::Vortex::Logging::GetCoreLogger()->trace(__VA_ARGS__)
#define VX_CORE_INFO(...)     ::Vortex::Logging::GetCoreLogger()->info(__VA_ARGS__)
#define VX_CORE_WARN(...)     ::Vortex::Logging::GetCoreLogger()->warn(__VA_ARGS__)
#define VX_CORE_ERROR(...)    ::Vortex::Logging::GetCoreLogger()->error(__VA_ARGS__)
#define VX_CORE_CRITICAL(...) ::Vortex::Logging::GetCoreLogger()->critical(__VA_ARGS__)

// Client logging macros
#define VX_TRACE(...)         ::Vortex::Logging::GetClientLogger()->trace(__VA_ARGS__)
#define VX_INFO(...)          ::Vortex::Logging::GetClientLogger()->info(__VA_ARGS__)
#define VX_WARN(...)          ::Vortex::Logging::GetClientLogger()->warn(__VA_ARGS__)
#define VX_ERROR(...)         ::Vortex::Logging::GetClientLogger()->error(__VA_ARGS__)
#define VX_CRITICAL(...)      ::Vortex::Logging::GetClientLogger()->critical(__VA_ARGS__)