#include "Vortex/PreHeaders.h"
#include "Vortex/Scene/Scene.h"
#include "Vortex/Scene/Component.h"
#include "Vortex/Scene/Entity.h"
#include "Graphics/Renderer2D.h"
#include "Graphics/MeshRenderer.h"

namespace Vortex
{

    void PrintMatrix(const glm::mat4 &matrix)
    {
        std::cout << std::fixed << std::setprecision(2);
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::defaultfloat;
    }


    Scene::Scene()
    {     
    }

    Scene::~Scene()
    {
        
    }

    Entity Scene::CreateEntity(const std::string &name)
    {
        Entity entity = { m_registry.create(), this };
        
        auto &tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Unnamed_Entity" : name;

        // Default Transform
        entity.AddComponent<TransformComponent>();

        // Default white texture
        Ref<Texture2D> texture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		texture->SetData(&whiteTextureData, sizeof(uint32_t));
        entity.AddComponent<TextureComponent>(texture, 10.0f);
        
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        m_registry.destroy(entity);
    }

    Entity Scene::GetPrimaryCameraEntity()
    {
        auto view = m_registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto camera_component = view.get<CameraComponent>(entity);
            if (camera_component.Primary)
            {
                return Entity { entity, this };
            }
        }
        return Entity{};
    }

    void Scene::OnUpdateEditor(TimeStep ts, EditorCamera &camera)
    {
        // Begin Simple Shape Scene
        // Renderer2D::BeginScene(camera);
        // auto GroupQuad = m_registry.group<TransformComponent, SpriteRendererComponent, TextureComponent>();
        // for (auto entity : GroupQuad)
        // {
        //     auto [transform, sprite, texture] = GroupQuad.get<TransformComponent, SpriteRendererComponent, TextureComponent>(entity);
        //     Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, (int)entity, texture.Texture, texture.TilingFactor);
        // }
        // Renderer2D::EndScene();

        // Begin Mesh Scene
        MeshRenderer::BeginScene(camera);
        auto GroupMesh = m_registry.group<MeshComponent, TransformComponent, SpriteRendererComponent, TextureComponent>();
        for (auto entity : GroupMesh)
        {
            auto [mesh, transform, sprite, texture] = GroupMesh.get<MeshComponent, TransformComponent, SpriteRendererComponent, TextureComponent>(entity);
            MeshRenderer::DrawMesh(mesh.MehsObj, transform.GetTransform(), sprite.Color, (int)entity, texture.Texture);
        }
        MeshRenderer::EndScene();
    }

    void Scene::OnUpdateRuntime(TimeStep ts)
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
            auto group = m_registry.group<TransformComponent, SpriteRendererComponent, TextureComponent>();

            for (auto entity : group)
            {
                auto [transform, sprite, texture] = group.get<TransformComponent, SpriteRendererComponent, TextureComponent>(entity);
                Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, (int)entity, texture.Texture, texture.TilingFactor);
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
    void Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component) {
        // Logic for SpriteRendererComponent
    }

    template<>
    void Scene::OnComponentAdded<TextureComponent>(Entity entity, TextureComponent& component) {
        // Logic for TextureComponent
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