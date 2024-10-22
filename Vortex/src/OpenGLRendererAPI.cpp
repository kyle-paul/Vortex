#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include <glm/glm.hpp>

namespace Vortex
{
    void OpenGLRendererAPI::Init()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    
    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

    void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::ClearBufferBit()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray> &VertexArrayID)
    {
        glDrawElements(GL_TRIANGLES, VertexArrayID->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
}