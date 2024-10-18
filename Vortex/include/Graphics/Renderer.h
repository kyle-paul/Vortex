#pragma once
#include "Graphics/RendererAPI.h"
#include "Graphics/RenderCommand.h"

namespace Vortex
{
    class Renderer
    {
    public:
        static void BeginScene();
        static void Submit(const std::shared_ptr<VertexArray>& VertexArrayID);
        static void EndScene();

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetCurrentAPI(); }
    };
}