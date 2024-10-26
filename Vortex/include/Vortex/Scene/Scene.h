#pragma once
#include <entt.hpp>
#include "Vortex/Core/TimeStep.h"
#include "Vortex/Core/Math.h"
#include "Vortex/Core/Math.h"

namespace Vortex
{
    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string &name = "");
        entt::registry& Reg() { return m_registry; }

        void OnUpdate(TimeStep ts);
        void OnViewPortResize(uint32_t width, uint32_t height);

    private:
        entt::registry m_registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        friend class Entity;
    };
}