#include "Graphics/Renderer2D.h"

namespace Vortex
{
    struct RendererManager
	{
		Ref<Shader> shad;
	};

	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		// Editor only
		int EntityID;
	};

    static RendererManager* manager;

    void Renderer2D::Init()
    {
		VX_PROFILE_FUNCTION();

        manager = new RendererManager();
		manager->shad = Shader::Create("TextureShader", "/home/pc/dev/engine/Sandbox/assets/Shaders/Custom.glsl");
		manager->shad->Bind();
		manager->shad->SetInt("u_Texture", 0);

    }

    void Renderer2D::Shutdown()
	{
		VX_PROFILE_FUNCTION();
		delete manager;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4 &transform) 
	{
		VX_PROFILE_FUNCTION();
		manager->shad->Bind();
		manager->shad->SetMat4("u_ViewProjection", camera.GetProjection() * glm::inverse(transform));
	}

	void Renderer2D::BeginScene(EditorCamera& camera) 
	{
		VX_PROFILE_FUNCTION();
		manager->shad->Bind();
		manager->shad->SetMat4("u_ViewProjection", camera.GetViewProjection());
	}

    void Renderer2D::EndScene()
	{
		VX_PROFILE_FUNCTION();
	}


	void Renderer2D::DrawShape(const Shape &shape, const glm::mat4 &transform, const glm::vec4 &color, const int EntityID, const Ref<Texture2D>& texture, const float tilingFactor)
	{
		manager->shad->SetFloat4("u_Color", color);
		manager->shad->SetMat4("u_Transform", transform);
		manager->shad->SetInt("u_EntityID", EntityID);
		manager->shad->SetFloat("u_TilingFactor", tilingFactor);
		
		texture->Bind();
		RenderCommand::DrawIndexed(shape.GetVertexArray());
	}

	void Renderer2D::DrawCircle(const glm::mat4 &transform, const glm::vec4 &color, const float Thickness, const float Fade, const int EntityID)
	{
		
	}
}