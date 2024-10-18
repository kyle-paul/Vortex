#pragma once
#include "Graphics/RendererAPI.h"

namespace Vortex
{
    class OpenGLRendererAPI : public RendererAPI
    {
        virtual void SetClearColor(const glm::vec4 &color) override;
        virtual void ClearBufferBit() override;
        virtual void DrawIndexed(const std::shared_ptr<VertexArray> &VertexArrayID) override;
    };
}