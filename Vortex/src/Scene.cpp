#include "Vortex/PreHeaders.h"
#include "Vortex/Scene/Scene.h"
#include "Vortex/Scene/Component.h"
#include "Vortex/Scene/Entity.h"
#include "Graphics/Renderer2D.h"

namespace Vortex
{
    static void DoMath(const glm::mat4 &transform)
    {

    }

    static void OnTransformConstruct(entt::registry &registry, entt::entity entity)
    {
        
    }

    Scene::Scene()
    {
        struct MeshComponent 
        {
            float value;
            MeshComponent() = default;
        };

        struct TransformComponent
        {
            glm::mat4 Transform;
            TransformComponent() = default;
            TransformComponent(const TransformComponent&) = default;
            TransformComponent(const glm::mat4 &transform) 
                : Transform(transform) { } 

            operator glm::mat4& () { return Transform; }
            operator const glm::mat4& () const { return Transform; }
        };

        TransformComponent transform_component;
        DoMath(glm::mat4(transform_component));

        // push back this entity as a TransformComponent type 
        // with initialization of glm::mat4(1.0f) into the registry
        entt::entity entity = m_registry.create();
        m_registry.emplace<TransformComponent>(entity, glm::mat4(1.0f)); 

        if (m_registry.has<TransformComponent>(entity))
            TransformComponent& transform = m_registry.get<TransformComponent>(entity);

        // iterate through all view to get entity
        auto view = m_registry.view<TransformComponent>();
        for (auto entity : view)
        {
            TransformComponent& transform = m_registry.get<TransformComponent>(entity);
        }        
    }

    Scene::~Scene()
    {
        
    }

    Entity Scene::CreateEntity(const std::string &name)
    {
        Entity entity = { m_registry.create(), this };
        entity.AddComponent<TransformComponent>(glm::mat4(1.0f));
        auto &tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Unnamed_Entity" : name;
        return entity;
    }

    void Scene::OnUpdate(TimeStep ts)
    {

        // Script update
        {
            m_registry.view<NativeScriptComponent>().each([=](auto entity, auto &nsc){
                if (!nsc.Instance)
                {
                    nsc.InstantiateFunction();
                    nsc.Instance->m_Entity = Entity { entity, this };
                    if (nsc.OnCreateFunction)
						nsc.OnCreateFunction(nsc.Instance);
                }
                if (nsc.OnUpdateFunction)
					nsc.OnUpdateFunction(nsc.Instance, ts);
            });
        }


        // Rendering
        Camera *MainCamera = nullptr;
        glm::mat4 *CameraTransform = nullptr;

        auto view = m_registry.view<TransformComponent, CameraComponent>();        
        for (auto entity : view)
        {
            const auto &[transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
            if (camera.Primary)
            {
                MainCamera = &camera.Camera;
                CameraTransform = &transform.Transform; // later -> inverse as view matrix
                break;
            }
        }

        // Begin rendering the scene
        if (MainCamera)
        {
            Renderer2D::BeginScene(*MainCamera, *CameraTransform);
            auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group)
            {
                const auto &[transform_s, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                Renderer2D::DrawQuad(transform_s.Transform, sprite.Color);
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
}