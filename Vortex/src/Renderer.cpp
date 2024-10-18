#include "Graphics/Renderer.h"

namespace Vortex 
{
    void Renderer::BeginScene() {

    }

    void Renderer::EndScene() {

    }

    void Renderer::Submit(const std::shared_ptr<VertexArray> &VertexArrayID) 
    {
        // RenderCommand::Init();
        RenderCommand::DrawIndexed(VertexArrayID);
    }
}