#include "Graphics/Renderer.h"

namespace Vortex 
{

    Renderer::SceneData *Renderer::m_SceneData = new Renderer::SceneData();

    void Renderer::Init()
    {
        RenderCommand::Init();
    }

    void Renderer::BeginScene(OrthographicCamera &camera) {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene() {

    }

    void Renderer::Submit(const Ref<Shader> &shader, 
                          const Ref<VertexArray>& VertexArrayID,
                          const glm::mat4 &transform)
    {
        shader->Bind();
        shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
        shader->SetMat4("u_Transform", transform);
        RenderCommand::DrawIndexed(VertexArrayID);
    }
}