#pragma once
#include <entt.hpp>
#include "Vortex/Core/TimeStep.h"
#include <glm/glm.hpp>
#include "Vortex/Core/Math.h"

namespace Vortex
{
    class Scene
    {
    public:
        Scene();
        ~Scene();

        entt::entity CreateEntity() { return  m_registry.create(); }
        entt::registry& Reg() { return m_registry; }

        void OnUpdate(TimeStep ts);

    private:
        entt::registry m_registry;
    };
}