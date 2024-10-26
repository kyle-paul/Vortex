#include "Vortex/Core/Core.h"
#include "Graphics/Renderer2D.h"
#include "Graphics/VertexArray.h"
#include "Graphics/Shader.h"
#include "Vortex/Core/Math.h"
#include "Graphics/RenderCommand.h"

namespace Vortex
{
	ImGuiComponents Renderer2D::m_ImGuiComponents;

    struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> shad;
		Ref<Texture2D> WhiteTexture;
	};

    static Renderer2DStorage* s_Data;

    void Renderer2D::Init()
    {
		VX_PROFILE_FUNCTION();

        s_Data = new Renderer2DStorage();
        s_Data->QuadVertexArray = VertexArray::Create();

        float squareVertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

        Ref<VertexBuffer> squareVB;
        squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TextCoord" }
		});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->shad = Shader::Create("TextureShader", "/home/pc/dev/engine/Sandbox/assets/Shaders/Custom.glsl");
		s_Data->shad->Bind();
		s_Data->shad->SetInt("u_Texture", 0);
    }

    void Renderer2D::Shutdown()
	{
		VX_PROFILE_FUNCTION();
		delete s_Data;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4 &transform) 
	{
		VX_PROFILE_FUNCTION();
		s_Data->shad->Bind();
		s_Data->shad->SetMat4("u_ViewProjection", camera.GetProjection() * glm::inverse(transform));
	}

	// TODO: remove this + imgui_components since the properties can be controlled in the sand box with entt
    // void Renderer2D::BeginScene(const OrthographicCamera& camera, ImGuiComponents &imgui_components) 
	// {
	// 	VX_PROFILE_FUNCTION();
	// 	m_ImGuiComponents = imgui_components;
	// 	s_Data->shad->Bind();
	// 	s_Data->shad->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	// }

    void Renderer2D::EndScene()
	{
		VX_PROFILE_FUNCTION();
	}

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		VX_PROFILE_FUNCTION();

		s_Data->shad->SetFloat4("u_Color", color);
		s_Data->WhiteTexture->Bind();

		s_Data->shad->SetMat4("u_Transform", TransformQuad(position, m_ImGuiComponents.ObjectRotation, size));

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		s_Data->shad->SetFloat4("u_Color", color);
		s_Data->WhiteTexture->Bind();

		s_Data->shad->SetMat4("u_Transform", transform);
		s_Data->QuadVertexArray->Bind();

		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		VX_PROFILE_FUNCTION();
		
		s_Data->shad->SetFloat4("u_Color", m_ImGuiComponents.BoardColor);
		s_Data->shad->SetFloat("u_TilingFactor", m_ImGuiComponents.TilingFactor);

		texture->Bind();

		s_Data->shad->SetMat4("u_Transform", TransformQuad(position, m_ImGuiComponents.BoardRotation, size));

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture)
	{
		s_Data->shad->SetFloat4("u_Color", m_ImGuiComponents.BoardColor);
		s_Data->shad->SetFloat("u_TilingFactor", m_ImGuiComponents.TilingFactor);

		texture->Bind();

		s_Data->shad->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	glm::mat4 Renderer2D::TransformQuad(const glm::vec3 &position, const float &rotation, const glm::vec2 &size)
	{
		return glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
		       glm::translate(glm::mat4(1.0f), position) *
			   glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
	}
}