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
        virtual void SetClearColor(const glm::vec4 &color) = 0;
        virtual void ClearBufferBit() = 0;
        virtual void DrawIndexed(const std::shared_ptr<VertexArray> &VertexArrayID) = 0;

        inline static API GetCurrentAPI() { return m_API; }

    private:
        static API m_API;
    };
}