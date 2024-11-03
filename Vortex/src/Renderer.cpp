#include "Vortex/Core/Core.h"
#include "Graphics/Renderer.h"
#include "Graphics/Renderer2D.h"
#include "Graphics/MeshRenderer.h"

namespace Vortex 
{

    Renderer::SceneData *Renderer::m_SceneData = new Renderer::SceneData();

    void Renderer::Init()
    {
        VX_PROFILE_FUNCTION();
        RenderCommand::Init();
        MeshRenderer::Init();
        // Renderer2D::Init();
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
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