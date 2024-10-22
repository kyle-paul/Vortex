#include <Vortex.h>
#include <imgui.h>
#include "glm/gtc/type_ptr.hpp"

#define VX_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

class ControlLayer : public Vortex::Layer  
{
public:
	ControlLayer() 
		: Layer("ControlLayer"), m_CameraController(1300.0f / 800.0f, true), m_SquarePosition(1.0f)
	{
		// ================================ Object 1 ================================
		m_VertexArraySquare.reset(Vortex::VertexArray::Create());
		
		// 1 Vertex Buffer
		float verticesSquare[4 * 5] = {
			-0.5f, -0.5, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};

		Vortex::Ref<Vortex::VertexBuffer> m_VertexBufferSquare;
		m_VertexBufferSquare.reset(Vortex::VertexBuffer::Create(verticesSquare, sizeof(verticesSquare)));

		// 2 Layout Buffer
		m_VertexBufferSquare->SetLayout({
			{  Vortex::ShaderDataType::Float3, "a_Position"  },
			{  Vortex::ShaderDataType::Float2, "a_TextCoord" }, 
		});
		m_VertexArraySquare->AddVertexBuffer(m_VertexBufferSquare);

		// 3. Index BUffer
		uint32_t indicesSquare[6] = { 0, 1, 2, 2, 3, 0 };
		Vortex::Ref<Vortex::IndexBuffer> m_IndexBufferSquare;
		m_IndexBufferSquare.reset(Vortex::IndexBuffer::Create(indicesSquare, 6));
		m_VertexArraySquare->SetIndexBuffer(m_IndexBufferSquare);

		// 4. Shader program
		auto m_ShaderSquare = m_ShaderLibrary.Load("square_shader", "assets/Shaders/Square.glsl");
		auto m_ShaderTexture = m_ShaderLibrary.Load("image_textures", "assets/Shaders/Custom.glsl");

		// 5. Texture
		m_TextureCheckBoard = Vortex::Texture2D::Create("assets/Textures/Checkerboard.png");
		m_TextureLogo = Vortex::Texture2D::Create("assets/Textures/ChernoLogo.png");
		m_ShaderTexture->Bind();
		m_ShaderTexture->SetInt("u_Texture", 0);
	}

	~ControlLayer()
	{

	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Color Control");
		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SqareColor));
		ImGui::End();
	}

	virtual void OnUpdate(Vortex::TimeStep ts) override 
	{
		// Key event controller for camera
		m_CameraController.OnUpdate(ts);

		Vortex::RenderCommand::SetClearColor({0, 0, 0, 1});
		Vortex::RenderCommand::ClearBufferBit();

		Vortex::Renderer::BeginScene(m_CameraController.GetCamera());
		
		// Get the texture from library
		auto m_ShaderSquare = m_ShaderLibrary.Get("square_shader");
		auto m_ShaderTexture = m_ShaderLibrary.Get("image_textures");

		// Transform
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		static glm::vec4 blue(0.2f, 0.3f, 0.8f, 1.0f);

		for (int i=0; i<5; i++) {
			glm::vec3 pos(i * 0.11f, 0.0f, 0.0f);
			for (int j=0; j<5; j++) {
				if (j % 2 == 0)	{
					m_ShaderSquare->SetFloat4("u_Color", m_SqareColor);
				}
				else {
					m_ShaderSquare->SetFloat4("u_Color", blue);
				}
				pos.y = j * 0.11f;
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarePosition + pos) * scale;
				Vortex::Renderer::Submit(m_ShaderSquare, m_VertexArraySquare, transform);
			}
		}

		// Bind and submit two different textures
		m_TextureCheckBoard->Bind();
		Vortex::Renderer::Submit(m_ShaderTexture, m_VertexArraySquare, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_TextureLogo->Bind();
		Vortex::Renderer::Submit(m_ShaderTexture, m_VertexArraySquare, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Vortex::Renderer::EndScene();
	}

	void OnEvent(Vortex::Event &event) override 
	{
		

	}

private:
	// Buffer and Shader
	Vortex::ShaderLibrary m_ShaderLibrary;
	Vortex::Ref<Vortex::VertexArray> m_VertexArray, m_VertexArraySquare;

	// Texture
	Vortex::Ref<Vortex::Texture2D> m_TextureCheckBoard, m_TextureLogo;

	// Camera component
	Vortex::OrthographicCameraController m_CameraController;
	// Vortex::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
	float m_CameraSpeed = 1.0f;
	float m_CameraRotation = 0.1f;
	float m_CameraRotationSpeed = 30.0f;

	// Object transform
	glm::vec3 m_SquarePosition;
	float m_SquareMoveSpeed = 1.5f;
	glm::vec4 m_SqareColor = {0.8f, 0.2f, 0.3f, 1.0f};
};


class Sandbox : public Vortex::Application 
{
public:
	Sandbox() {
		PushLayer(new ControlLayer());
	}

	~Sandbox () {

	}
};

Vortex::Application* Vortex::CreateApplication()
{
	return new Sandbox();
}