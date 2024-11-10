#pragma once
#include "Graphics/RendererAPI.h"

namespace Vortex
{
    class OpenGLRendererAPI : public RendererAPI
    {
        virtual void Init() override;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        virtual void SetClearColor(const glm::vec4 &color) override;
        virtual void ClearBufferBit() override;

        virtual void DrawIndexed(const Ref<VertexArray> &VertexArrayID) override;
        virtual void DrawIndexedMulti(const Ref<VertexArray>& vertexArray, uint32_t indexCount) override;
    };
}