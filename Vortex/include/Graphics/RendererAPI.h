#pragma once
#include <glm/glm.hpp>
#include "Graphics/VertexArray.h"

namespace Vortex
{
    class RendererAPI
    {
    public:
        enum class API
        {
            None = 0,
            OpenGL = 1,
            VulCan = 2,
            DirectX = 3,
        };

        virtual void Init() = 0;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
        virtual void SetClearColor(const glm::vec4 &color) = 0;
        virtual void ClearBufferBit() = 0;

        virtual void DrawIndexed(const Ref<VertexArray> &VertexArrayID) = 0;
        virtual void DrawIndexedMulti(const Ref<VertexArray>& vertexArray, uint32_t indexCount) = 0;

        inline static API GetCurrentAPI() { return m_API; }

    private:
        static API m_API;
    };
}