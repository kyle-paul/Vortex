#pragma once
#include <glm/glm.hpp>
#include "Graphics/OrthographicCamera.h"
#include "Graphics/Camera.h"

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
        glm::mat4 Transform;
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4 &transform) 
            : Transform(transform) { } 

        operator glm::mat4& () { return Transform; }
        operator const glm::mat4& () const { return Transform; }
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
        Vortex::Camera Camera;
        bool Primary = true;
        
        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
        CameraComponent(const glm::mat4 projection)
            :Camera(projection) { }
    };
};