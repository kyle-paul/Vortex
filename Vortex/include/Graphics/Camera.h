#pragma once
#include <glm/glm.hpp>

namespace Vortex
{
    class Camera
    {
    public:
        Camera(const glm::mat4 &projection)
            :m_projection(projection)
        {
            
        }

        inline const glm::mat4 GetProjection() const { return m_projection; }
        
    private:
        glm::mat4 m_projection;
    };
}