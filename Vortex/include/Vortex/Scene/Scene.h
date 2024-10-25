#pragma once
#include <entt.hpp>
#include "Vortex/Core/TimeStep.h"
#include <glm/glm.hpp>
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

    private:
        entt::registry m_registry;

        friend class Entity;
    };
}