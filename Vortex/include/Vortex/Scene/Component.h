#pragma once
#include "Graphics/Texture.h"
#include "Graphics/Mesh.h"
#include "Graphics/Shape.h"
#include "Vortex/Scene/SceneCamera.h"
#include "Vortex/Scene/ScriptableEntity.h"
#include "Vortex/Core/TimeStep.h"
#include <Vortex/Core/Math.h>

namespace Vortex
{
    struct TagComponent
    {
        std::string Tag;
        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string &tag)
            : Tag(tag) { }
    };

    struct TransformComponent
    {
        glm::vec3 Translation = { 0.0f, 0.0f, 0.0f }; 
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Scale = {1.0f, 1.0f, 1.0f };        

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3 &translation) 
            : Translation(translation) { } 

        glm::mat4 GetTransform() const
        {
            glm::mat4 rotation_matrix = glm::toMat4(glm::quat(Rotation));
            return glm::translate(glm::mat4(1.0f), Translation) * 
                   rotation_matrix * glm::scale(glm::mat4(1.0f), Scale);
        }
    };

    struct MeshComponent 
    {
        Mesh MeshObj;
        bool Distorted = false;

        MeshComponent() = default;
        MeshComponent(const MeshComponent&) = default;
        MeshComponent(const Mesh &mesh_obj)
            : MeshObj(mesh_obj) { }
    };

    struct ShapeComponent
    {
        Shape ShapeObj;

        ShapeComponent() = default;
        ShapeComponent(const ShapeComponent&) = default;
        ShapeComponent(const Shape &shape)
            : ShapeObj(shape) { }
    };

    struct TextureComponent
    {
        Ref<Texture2D> Texture;
        float TilingFactor;

        TextureComponent() = default;
        TextureComponent(const TextureComponent&) = default;
        TextureComponent(Ref<Texture2D> texture, float tiling_factor) :
            Texture(texture), TilingFactor(tiling_factor) { }
    };

    struct SpriteRendererComponent
    {
        glm::vec4 Color = {1.0f, 0.0f, 0.0f, 1.0f};
        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4 &color) 
            : Color(color) { } 
    };

    struct CameraComponent
    {
        SceneCamera Camera;
        bool Primary = true;
        bool FixedAspectRatio = false;
        
        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };

    struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

    struct Rigidbody2DComponent
    {
        enum class BodyType { Static = 0, Dynamic, Kinematic };
        BodyType Type = BodyType::Static;

        bool FixedRotation = false;  
        void* RuntimeBody = nullptr; // Storage for runtime

        Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
    };

    struct BoxCollider2DComponent
    {
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

        // Move into physics material in the future maybe
        float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

        // Storage for runtime
        void* RuntimeFixture = nullptr;

        BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
    };
};