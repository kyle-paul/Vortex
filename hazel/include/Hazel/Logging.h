#pragma once
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Hazel 
{
    class Logging 
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
