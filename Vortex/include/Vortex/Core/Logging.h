#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#include <memory>


namespace Vortex 
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

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
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