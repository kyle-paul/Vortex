#include "Vortex/Scene/SceneCamera.h"
#include "Vortex/Core/Assert.h"

namespace Vortex
{
    SceneCamera::SceneCamera()
    {
        RecalculateProjection();
    }

    void SceneCamera::SetOrthographic(float size, float near, float far)
    {
        m_OrthographicSize = size;
        m_OrthographicNear = near;
        m_OrthographicFar = far;
        RecalculateProjection();
    }

    void SceneCamera::SetViewPortSize(uint32_t width, uint32_t height)
    {
        VX_CORE_ASSERT(width > 0 && height > 0);
        m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
    }
    
    void SceneCamera::RecalculateProjection()
    {
        float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
        float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
        float orthoBottom = -m_OrthographicSize * 0.5f;
        float orthoTop = m_OrthographicSize * 0.5f;

        m_projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
    }
}