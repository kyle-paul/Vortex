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

        static inline void DrawIndexed(const Ref<VertexArray> &VertexArrayID, const std::string &type)
        {
            VertexArrayID->Bind();
            m_RendererAPI->DrawIndexed(VertexArrayID, type);
        }

        static inline void DrawIndexedMulti(const Ref<VertexArray> &VertexArrayID, uint32_t count=0)
        {
            VertexArrayID->Bind();
            m_RendererAPI->DrawIndexedMulti(VertexArrayID, count);
        }
        
    public:
        static RendererAPI *m_RendererAPI;
    };
}