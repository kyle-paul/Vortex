#include "Vortex/Core/Core.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include <glm/glm.hpp>

namespace Vortex
{
    void OpenGLRendererAPI::Init()
    {
        VX_PROFILE_FUNCTION();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
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

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray> &VertexArrayID, const std::string &type)
    {
        if (type == "Quad")
            glDrawElements(GL_QUADS, VertexArrayID->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        else
            glDrawElements(GL_TRIANGLES, VertexArrayID->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        // glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLRendererAPI::DrawIndexedMulti(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		// glBindTexture(GL_TEXTURE_2D, 0);
	}
}