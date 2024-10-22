#pragma once
#include "Graphics/RendererAPI.h"

namespace Vortex
{
    class RenderCommand
    {
    public:
        static inline void Init()
        {
            m_RendererAPI->Init();
        }

        inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			m_RendererAPI->SetViewport(x, y, width, height);
		}

        static inline void SetClearColor(const glm::vec4 &color) {
            m_RendererAPI->SetClearColor(color);
        }

        static inline void ClearBufferBit() {
            m_RendererAPI->ClearBufferBit();
        }

        static inline void DrawIndexed(const std::shared_ptr<VertexArray> &VertexArrayID)
        {
            VertexArrayID->Bind();
            return m_RendererAPI->DrawIndexed(VertexArrayID);
        }
        
    public:
        static RendererAPI *m_RendererAPI;
    };
}