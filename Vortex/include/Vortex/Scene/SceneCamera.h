#pragma once
#include "Graphics/Camera.h"
#include "Vortex/Core/Math.h"

namespace Vortex
{
    class SceneCamera : public Camera
    {
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;
        void SetOrthographic(float size, float near, float far);
        void SetViewPortSize(uint32_t width, uint32_t height);

        float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }

    private:
        void RecalculateProjection();
    
    private:
        float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
		float m_AspectRatio = 0.0f;
    };
}