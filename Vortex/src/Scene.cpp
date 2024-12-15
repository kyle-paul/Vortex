#include "Vortex/PreHeaders.h"
#include "Vortex/Scene/Scene.h"
#include "Vortex/Scene/Component.h"
#include "Vortex/Scene/Entity.h"
#include "Graphics/Renderer2D.h"
#include "Graphics/RenderMulti.h"
#include "Graphics/MeshRenderer.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

namespace Vortex
{
    static b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodytype)
    {
        switch(bodytype)
        {
            case Rigidbody2DComponent::BodyType::Static: return b2_staticBody;
            case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
            case Rigidbody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
        }
        VX_CORE_ASSERT(false, "Unknown Body Type, return static body type as default");
        return b2_staticBody;
    }

    Scene::Scene()
    {     
    }

    Scene::~Scene()
    {
        
    }

    template<typename Component>
    static void CopyComponent(entt::registry &DestReg, entt::registry &SrcReg, const std::unordered_map<UUID, entt::entity> &enttMap)
    {
        auto view = SrcReg.view<Component>();
        for (auto e : view)
        {
            UUID uuid = SrcReg.get<IDComponent>(e).ID;
            VX_CORE_ASSERT(enttMap.find(uuid) != enttMap.end());
            entt::entity dstEnttID = enttMap.at(uuid);

            auto &ComponentData = SrcReg.get<Component>(e);
            DestReg.emplace_or_replace<Component>(dstEnttID, ComponentData);
            // emplace in the registry the index of Entity = entt:entity
            // the data information of the desirec copied component
        }
    }
    
    template<typename Component>
    static void CopyComponentIfExists(Entity DesEntity, Entity SrcEntity)
    {
        if (SrcEntity.HasComponent<Component>())
        {
            DesEntity.AddOrReplaceComponent<Component>(SrcEntity.GetComponent<Component>());
        }
    }
    
    Ref<Scene> Scene::Copy(Ref<Scene> OtherScene)
    {
        Ref<Scene> NewScene = CreateRef<Scene>();
     
        NewScene->m_ViewportWidth = OtherScene->m_ViewportWidth;
        NewScene->m_ViewportHeight = OtherScene->m_ViewportHeight;

        auto &srcSceneRegistry = OtherScene->m_registry;
        auto &dstSceneRegistry = NewScene->m_registry;

        std::unordered_map<UUID, entt::entity> enttMap;

        auto idView = srcSceneRegistry.view<IDComponent>();
        for (auto e : idView)
        {
            UUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
            const auto &name = srcSceneRegistry.get<TagComponent>(e).Tag;
            Entity newEntity = NewScene->CreateEntityWithUUID(uuid, name);
            enttMap[uuid] = (entt::entity)newEntity; // map <uuid, index of Entity class = entt:entity>
        }

        // Copy Components
        CopyComponent<TransformComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<ShapeComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<MeshComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<CircleComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<SpriteRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<CameraComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<NativeScriptComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<Rigidbody2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<BoxCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);

        return NewScene;
    }

    void Scene::DuplicateEntity(Entity entity)
	{
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);
		CopyComponentIfExists<TransformComponent>(newEntity, entity);
		CopyComponentIfExists<ShapeComponent>(newEntity, entity);
		CopyComponentIfExists<MeshComponent>(newEntity, entity);
        CopyComponentIfExists<CircleComponent>(newEntity, entity);
		CopyComponentIfExists<SpriteRendererComponent>(newEntity, entity);
		CopyComponentIfExists<CameraComponent>(newEntity, entity);
		CopyComponentIfExists<NativeScriptComponent>(newEntity, entity);
		CopyComponentIfExists<Rigidbody2DComponent>(newEntity, entity);
		CopyComponentIfExists<BoxCollider2DComponent>(newEntity, entity);

        newEntity.GetComponent<TransformComponent>().Translation.x += 0.1f;
	}

    void Scene::OnRuntimeStart()
    {
        m_PhysicsWorld = new b2World({ 0.0f, -9.8f });  // The gravity

		auto view = m_registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;
			
			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
    }
        
    void Scene::OnRuntimeStop()
    {
        delete m_PhysicsWorld;
        m_PhysicsWorld = nullptr;
    }

    Entity Scene::CreateEntity(const std::string &name)
    {
        return CreateEntityWithUUID(UUID(), name);
    }

    Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string &name)
    {
        Entity entity = { m_registry.create(), this };

        // UUID
        entity.AddComponent<IDComponent>(uuid);

        // Default tag name
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
        // Begin Scene
        Renderer2D::BeginScene(camera);
        MeshRenderer::BeginScene(camera);
        // RenderMulti::BeginScene(camera);

        // Single group with shared components
        auto view = m_registry.view<TransformComponent, SpriteRendererComponent, TextureComponent>();

        for (auto entity : view)
        {
            auto [transform, sprite, texture] = view.get<TransformComponent, SpriteRendererComponent, TextureComponent>(entity);
            
            // Batch Rendering
            // if (m_registry.has<BatchComponent>(entity))
            // {
            //     auto& batch = m_registry.get<BatchComponent>(entity); 
            //     RenderMulti::DrawShape(batch.index, transform.GetTransform(), sprite.Color, (int)entity, texture.Texture, texture.TilingFactor);
            // }

            // ShapeComponent
            if (m_registry.has<ShapeComponent>(entity))
            {
                auto& shape = m_registry.get<ShapeComponent>(entity);
                Renderer2D::DrawShape(shape.ShapeObj, transform.GetTransform(), sprite.Color, (int)entity, texture.Texture, texture.TilingFactor);
            }
            
            // MeshComponent
            if (m_registry.has<MeshComponent>(entity))
            {
                auto& mesh = m_registry.get<MeshComponent>(entity);
                MeshRenderer::DrawMesh(mesh.MeshObj, transform.GetTransform(), sprite.Color, (int)entity, texture.Texture, texture.TilingFactor, mesh.Type);
            }

            // if (m_registry.has<CircleComponent>(entity))
            // {
            //     auto& circle = m_registry.get<CircleComponent>(entity);
            //     Renderer2D::DrawCircle(transform.GetTransform(), sprite.Color, circle.Thickness, circle.Fade, (int)entity);
            // }
        }

        // End Scene
        // RenderMulti::EndScene();
        MeshRenderer::EndScene();
        Renderer2D::EndScene();
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

        // Physics
		{            
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

			// Retrieve transform from Box2D
			auto view = m_registry.view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& trans_comp = entity.GetComponent<TransformComponent>();
				auto& rb2d_comp = entity.GetComponent<Rigidbody2DComponent>();

				b2Body* body = (b2Body*)rb2d_comp.RuntimeBody;
				const auto& position = body->GetPosition();
				trans_comp.Translation.x = position.x;
				trans_comp.Translation.y = position.y;
				trans_comp.Rotation.z = body->GetAngle();
			}
		}

        // Begin rendering the scene
        if (MainCamera)
        {
            // Begin Scene
            Renderer2D::BeginScene(*MainCamera, CameraTransform);
            MeshRenderer::BeginScene(*MainCamera, CameraTransform);
            // RenderMulti::BeginScene(*MainCamera, CameraTransform);

            // Single group with shared components
            auto view = m_registry.view<TransformComponent, SpriteRendererComponent, TextureComponent>();

            for (auto entity : view)
            {
                auto [transform, sprite, texture] = view.get<TransformComponent, SpriteRendererComponent, TextureComponent>(entity);

                // Batch Rendering
                // if (m_registry.has<BatchComponent>(entity))
                // {
                //     auto& batch = m_registry.get<BatchComponent>(entity); 
                //     RenderMulti::DrawShape(batch.index, transform.GetTransform(), sprite.Color, (int)entity, texture.Texture, texture.TilingFactor);
                // }
                
                // ShapeComponent
                if (m_registry.has<ShapeComponent>(entity))
                {
                    auto& shape = m_registry.get<ShapeComponent>(entity);
                    Renderer2D::DrawShape(shape.ShapeObj, transform.GetTransform(), sprite.Color, (int)entity, texture.Texture, texture.TilingFactor);
                }
                
                // MeshComponent
                if (m_registry.has<MeshComponent>(entity))
                {
                    auto& mesh = m_registry.get<MeshComponent>(entity);
                    MeshRenderer::DrawMesh(mesh.MeshObj, transform.GetTransform(), sprite.Color, (int)entity, texture.Texture, texture.TilingFactor, mesh.Type);
                }

                // CircleComponent
                // if (m_registry.has<CircleComponent>(entity))
                // {
                //     auto& circle = m_registry.get<CircleComponent>(entity);
                //     Renderer2D::DrawCircle(transform.GetTransform(), sprite.Color, circle.Thickness, circle.Fade, (int)entity);
                // }
            }

            // End Scene
            // RenderMulti::EndScene();
            MeshRenderer::EndScene();
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
    void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component) {
        // Logic for IDComponent
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
        // Logic for MeshComponent
    }

    template<>
    void Scene::OnComponentAdded<ShapeComponent>(Entity entity, ShapeComponent& component) {
        // Logic for ShapeComponent
    }

    template<>
    void Scene::OnComponentAdded<BatchComponent>(Entity entity, BatchComponent& component) {
        // Logic for BatchComponent
    }

    template<>
    void Scene::OnComponentAdded<CircleComponent>(Entity entity, CircleComponent& component) {
        // Logic for CircleComponent
    }

    template<>
    void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component) {
        // Logic for Rigidbody2DComponent
    }

    template<>
    void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component) {
        // Logic for BoxCollider2DComponent
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