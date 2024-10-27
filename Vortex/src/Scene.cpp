#include "Vortex/PreHeaders.h"
#include "Vortex/Scene/Scene.h"
#include "Vortex/Scene/Component.h"
#include "Vortex/Scene/Entity.h"
#include "Graphics/Renderer2D.h"

namespace Vortex
{
    Scene::Scene()
    {     
    }

    Scene::~Scene()
    {
        
    }

    Entity Scene::CreateEntity(const std::string &name)
    {
        Entity entity = { m_registry.create(), this };
        entity.AddComponent<TransformComponent>();
        auto &tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Unnamed_Entity" : name;
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        m_registry.destroy(entity);
    }

    void Scene::OnUpdate(TimeStep ts)
    {

        // Script update
        {
			m_registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				// TODO: Move to Scene::OnScenePlay
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}
				nsc.Instance->OnUpdate(ts);
			});
		}

        // Rendering
        Camera *MainCamera = nullptr;
        glm::mat4 CameraTransform;

        auto view = m_registry.view<TransformComponent, CameraComponent>();        
        for (auto entity : view)
        {
            auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
            if (camera.Primary)
            {
                MainCamera = &camera.Camera;
                CameraTransform = transform.GetTransform(); // later -> inverse as view matrix
                break;
            }
        }

        // Begin rendering the scene
        if (MainCamera)
        {
            Renderer2D::BeginScene(*MainCamera, CameraTransform);
            auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group)
            {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
            }
            Renderer2D::EndScene();
        }
    }

    void Scene::OnViewPortResize(uint32_t width, uint32_t height)
    {
        if (m_ViewportWidth == width && m_ViewportHeight == height)
			return;

		m_ViewportWidth = width;
		m_ViewportHeight = height;

        // Resize our non-FixedAspectRatio cameras
		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
            {
				cameraComponent.Camera.SetViewPortSize(width, height);
            }
		}
    }   


    template<>
    void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {
        // Logic for TransformComponent
    }

    template<>
    void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {
        component.Camera.SetViewPortSize(m_ViewportWidth, m_ViewportHeight);
    }

    template<>
    void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {
        // Logic for SpriteRendererComponent
    }

    template<>
    void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) {
        // Logic for TagComponent, if needed
    }

    template<>
    void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {
        // Logic for NativeScriptComponent, if needed
    }
}