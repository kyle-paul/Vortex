#include "Vortex/Core/Core.h"
#include "Graphics/Renderer.h"
#include "Graphics/Renderer2D.h"
#include "Graphics/RenderMulti.h"
#include "Graphics/MeshRenderer.h"

namespace Vortex 
{

    Renderer::SceneData *Renderer::m_SceneData = new Renderer::SceneData();

    void Renderer::Init()
    {
        VX_PROFILE_FUNCTION();
        RenderCommand::Init();
        Renderer2D::Init();
        MeshRenderer::Init();
        // RenderMulti::Init();
    }

    void Renderer::Shutdown()
    {
        Renderer2D::Shutdown();
        MeshRenderer::Shutdown();
        // RenderMulti::Shutdown();
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
    }
}