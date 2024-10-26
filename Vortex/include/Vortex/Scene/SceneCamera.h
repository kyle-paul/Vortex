#pragma once
#include "Graphics/Camera.h"
#include "Vortex/Core/Math.h"

namespace Vortex
{
    class SceneCamera : public Camera
    {
    public:
        enum class ProjectionType { Perspective = 0, Orthographic = 1 };

        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetOrthographic(float size, float near, float far);
        void SetPerspective(float verticalFOV, float nearClip, float farClip);
        void SetViewPortSize(uint32_t width, uint32_t height);

        // Get Perspective Properties
        float GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
        float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
        float GetPerspectiveFarClip() const { return m_PerspectiveFar; }

        // Get Orthographic Properties
        float GetOrthographicSize() const { return m_OrthographicSize; }
        float GetOrthographicNearClip() const { return m_OrthographicNear; }
        float GetOrthographicFarClip() const { return m_OrthographicFar; }

        // Set Perspective Properties
        void SetPerspectiveVerticalFOV(float verticalFov) { m_PerspectiveFOV = verticalFov; RecalculateProjection(); }
        void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
        void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }

        // Set Orthographic Properties
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
        void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
        void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }
        
        // Projection type
        inline ProjectionType GetProjectionType() const { return m_ProjectionType; }
        inline void SetProjectionType(ProjectionType type) { m_ProjectionType = type; } 

    private:
        void RecalculateProjection();
    
    private:
        // Projection type
        ProjectionType m_ProjectionType = ProjectionType::Orthographic;

        // Perspective properties
        float m_PerspectiveFOV = glm::radians(45.0f);
        float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

        // Orthographic propertie
        float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
        float m_AspectRatio = 0.0f;
    };
}