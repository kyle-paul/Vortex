#pragma once
#include <entt.hpp>
#include "Vortex/Core/TimeStep.h"
#include "Vortex/Core/Math.h"
#include "Graphics/EditorCamera.h"


class b2World;


namespace Vortex
{
    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string &name = "");
        void DestroyEntity(Entity entity);

        entt::registry& Reg() { return m_registry; }

        void OnUpdateEditor(TimeStep ts, EditorCamera &camera);
        void OnUpdateRuntime(TimeStep ts);
        void OnViewPortResize(uint32_t width, uint32_t height);

        void OnRuntimeStart();
        void OnRuntimeStop();

        Entity GetPrimaryCameraEntity();

   	public:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

    private:
        entt::registry m_registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        b2World* m_PhysicsWorld = nullptr;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };

    struct TransformComponent;
    struct CameraComponent;
    struct SpriteRendererComponent;
    struct TextureComponent;
    struct TagComponent;
    struct NativeScriptComponent;
}