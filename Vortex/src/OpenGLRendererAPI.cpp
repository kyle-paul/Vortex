#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include <glm/glm.hpp>

namespace Vortex
{
    void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::ClearBufferBit()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray> &VertexArrayID)
    {
        glDrawElements(GL_TRIANGLES, VertexArrayID->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
}