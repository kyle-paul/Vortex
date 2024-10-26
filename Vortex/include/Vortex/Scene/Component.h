#pragma once
#include <glm/glm.hpp>
#include "Graphics/OrthographicCamera.h"
#include "Vortex/Scene/SceneCamera.h"
#include "Vortex/Scene/ScriptableEntity.h"
#include "Vortex/Core/TimeStep.h"

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
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f }; // Radians => for serialization purpose
        glm::vec3 scale = {1.0f, 1.0f, 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3 &translation) 
            : Translation(translation) { } 

        glm::mat4 GetTransform() const
        {
            glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), Rotation.x, {1, 0, 0}) *
                                       glm::rotate(glm::mat4(1.0f), Rotation.y, {0, 1, 0}) *
                                       glm::rotate(glm::mat4(1.0f), Rotation.z, {0, 0, 1});
            return glm::translate(glm::mat4(1.0f), Translation) * 
                   rotation_matrix * glm::scale(glm::mat4(1.0f), scale);
        }
    };

    struct SpriteRendererComponent
    {
        glm::vec4 Color;
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
};