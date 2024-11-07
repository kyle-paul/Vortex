#pragma once

#include <entt.hpp>
#include "Vortex/Scene/Scene.h"
#include "Vortex/Core/Core.h"
#include "Vortex/Core/UUID.h"
#include "Vortex/Scene/Component.h"

namespace Vortex
{
    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity &other) = default;

        template<typename T, typename... Args> 
        T& AddComponent(Args&&... args)
        {
            VX_CORE_ASSERT(!HasComponent<T>(), "This Entity already had this component!");\
            T& component = m_Scene->m_registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
            return component;
        }

        template<typename T>
        T& GetComponent()
        {
            VX_CORE_ASSERT(HasComponent<T>(), "This Entity does not have this component to get!");
            return m_Scene->m_registry.get<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent()
        {
            return m_Scene->m_registry.has<T>(m_EntityHandle);
        }

        template<typename T>
        void RemoveComponent()
        {
            VX_CORE_ASSERT(HasComponent<T>(), "This Entity does not have this component to remove!");
            return m_Scene->m_registry.remove<T>(m_EntityHandle);
        }

        operator bool() const { return m_EntityHandle != entt::null; }
        operator uint32_t() const { return (uint32_t)(m_EntityHandle); }
        operator entt::entity() const { return m_EntityHandle; }
        
        UUID GetUUID() { return GetComponent<IDComponent>().ID; }

        bool operator==(const Entity &other) const { 
            return m_EntityHandle == other.m_EntityHandle && 
                   m_Scene == other.m_Scene;
        }

        bool operator!=(const Entity &other) const { 
            return !(*this == other);
            return !operator==(other);
        }

    private:
        entt::entity m_EntityHandle {entt::null};
        Scene* m_Scene = nullptr;
    };
}