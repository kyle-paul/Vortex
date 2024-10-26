#pragma once
#include <glm/glm.hpp>

namespace Vortex
{
    class Camera
    {
    public:
        Camera() = default;
        Camera(const glm::mat4 &projection)
            :m_projection(projection) { }

        virtual ~Camera() = default;

        inline const glm::mat4 GetProjection() const { return m_projection; }
        inline void SetProjection(const glm::mat4& projection_matrix) { m_projection = projection_matrix; }
        
    protected:
        glm::mat4 m_projection = glm::mat4(1.0f);
    };
}