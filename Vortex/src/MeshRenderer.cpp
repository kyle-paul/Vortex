#include "Graphics/MeshRenderer.h"

namespace Vortex
{
    struct RenderMeshManagaer 
    {
        Ref<Shader> shad;
    };    

    static RenderMeshManagaer* manager;

    MeshRenderer::MeshRenderer()
    {
        
    }   

    void MeshRenderer::Init()
    {
        manager = new RenderMeshManagaer();
        manager->shad = Shader::Create("MeshShader", "/home/pc/dev/engine/Sandbox/assets/Shaders/Custom.glsl");
		manager->shad->Bind();
		manager->shad->SetInt("u_Texture", 0);
    }

    MeshRenderer::~MeshRenderer()
    {
        delete manager;
    }

    void MeshRenderer::BeginScene(const Camera& camera, const glm::mat4 &transform)
    {
        manager->shad->Bind();
        manager->shad->SetMat4("u_ViewProjection", camera.GetProjection() * glm::inverse(transform));
    }

    void MeshRenderer::BeginScene(EditorCamera& camera)
    {
        manager->shad->Bind();
        manager->shad->SetMat4("u_ViewProjection", camera.GetViewProjection());
    }

    void MeshRenderer::DrawMesh(const Mesh &mesh, const glm::mat4& transform, const glm::vec4& color, const int EntityID, const Ref<Texture2D>& texture, const float tilingFactor)
    {
        manager->shad->SetFloat4("u_Color", color);
		manager->shad->SetMat4("u_Transform", transform);
		manager->shad->SetInt("u_EntityID", EntityID);
        manager->shad->SetFloat("u_TilingFactor", tilingFactor);

        texture->Bind();
        RenderCommand::DrawIndexed(mesh.GetVertexArray());
    }

    void MeshRenderer::EndScene()
	{
	}
}