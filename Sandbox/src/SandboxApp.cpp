#include <Vortex.h>
#include <Vortex/Core/Math.h>
#include <imgui.h>
#include "glm/gtc/type_ptr.hpp"

#define VX_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

class ControlLayer : public Vortex::Layer  
{
public:
	ControlLayer() 
		: Layer("ControlLayer"), m_Camera(-1.625, 1.625f, -1.0f, 1.0f), m_SquarePosition(1.0f)
	{
		// ================================ Object 1 ================================
		m_VertexArray.reset(Vortex::VertexArray::Create());
		
		// 1 Vertex Buffer
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f,  0.2f, 0.3f, 0.8f, 1.0f,
			0.0f, 0.5f, 0.0f,   0.8f, 0.8f, 0.2f, 1.0f 
		};

		std::shared_ptr<Vortex::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(Vortex::VertexBuffer::Create(vertices, sizeof(vertices)));
		
		// 2 Layout Buffer
		m_VertexBuffer->SetLayout({
			{  Vortex::ShaderDataType::Float3, "a_Position" },
			{  Vortex::ShaderDataType::Float4, "a_Color"},
		});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		// 3. Index BUffer
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Vortex::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(Vortex::IndexBuffer::Create(indices, 3));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		// 4. Shader program
		std::string vertexSource = getVertexSource();
		std::string fragmentSource = getFragmentSource();
		m_Shader.reset(Vortex::Shader::Create("triangle_shader", vertexSource, fragmentSource));


		// ================================ Object 2 ================================
		m_VertexArraySquare.reset(Vortex::VertexArray::Create());
		
		// 1 Vertex Buffer
		float verticesSquare[4 * 3] = {
			-0.5f, -0.5, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};

		std::shared_ptr<Vortex::VertexBuffer> m_VertexBufferSquare;
		m_VertexBufferSquare.reset(Vortex::VertexBuffer::Create(verticesSquare, sizeof(verticesSquare)));

		// 2 Layout Buffer
		m_VertexBufferSquare->SetLayout({
			{  Vortex::ShaderDataType::Float3, "a_Position" },
		});
		m_VertexArraySquare->AddVertexBuffer(m_VertexBufferSquare);

		// 3. Index BUffer
		uint32_t indicesSquare[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Vortex::IndexBuffer> m_IndexBufferSquare;
		m_IndexBufferSquare.reset(Vortex::IndexBuffer::Create(indicesSquare, 6));
		m_VertexArraySquare->SetIndexBuffer(m_IndexBufferSquare);

		// 4. Shader program
		std::string vertexSourceSquare = getVertexSourceSquare();
		std::string fragmentSourceSquare = getFragmentSourceSquare();
		m_ShaderSquare.reset(Vortex::Shader::Create("square_shader", vertexSourceSquare, fragmentSourceSquare));
	}

	~ControlLayer()
	{

	}

	std::string getVertexSource() {
		return R"(
			#version 330 core
			
			layout(location=0) in vec3 a_Position;
			layout(location=1) in vec4 a_Color;
			
			out vec4 v_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main() {
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";
	}
	
	std::string getFragmentSource() {
		return R"(
			#version 330 core
			
			in vec4 v_Color;
			layout(location=0) out vec4 color;

			void main() {
				color = v_Color;
			}
		)";
	}

	std::string getVertexSourceSquare() {
		return R"(
			#version 330 core
			
			layout(location=0) in vec3 a_Position;
			out vec3 v_Position;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
			void main() {
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";
	}
	
	std::string getFragmentSourceSquare() {
		return R"(
			#version 330 core
			
			in vec3 v_Position;
			layout(location=0) out vec4 color;
			uniform vec4 u_Color;

			void main() {
				color = u_Color;
			}
		)";
	}
	

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Color Control");
		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SqareColor));
		ImGui::End();
	}

	virtual void OnUpdate(Vortex::TimeStep ts) override 
	{
		if (Vortex::Input::IsKeyPressed(Vortex::Key::Left)) {
			m_CameraPosition.x -= m_CameraSpeed * float(ts);
		}

		if (Vortex::Input::IsKeyPressed(Vortex::Key::Right)) {
			m_CameraPosition.x += m_CameraSpeed * float(ts);
		}

		if (Vortex::Input::IsKeyPressed(Vortex::Key::Up)) {
			m_CameraPosition.y += m_CameraSpeed * float(ts);
		}

		if (Vortex::Input::IsKeyPressed(Vortex::Key::Down)) {
			m_CameraPosition.y -= m_CameraSpeed * float(ts);
		}

		if (Vortex::Input::IsKeyPressed(Vortex::Key::A)) {
			m_CameraRotation += m_CameraRotationSpeed * float(ts);
		}
		
		else if (Vortex::Input::IsKeyPressed(Vortex::Key::D)) {
			m_CameraRotation -= m_CameraRotationSpeed * float(ts);
		}


		if (Vortex::Input::IsKeyPressed(Vortex::Key::J)) {
			m_SquarePosition.x -= m_SquareMoveSpeed * float(ts);
		}

		if (Vortex::Input::IsKeyPressed(Vortex::Key::L)) {
			m_SquarePosition.x += m_SquareMoveSpeed * float(ts);
		}

		if (Vortex::Input::IsKeyPressed(Vortex::Key::I)) {
			m_SquarePosition.y += m_SquareMoveSpeed * float(ts);
		}

		if (Vortex::Input::IsKeyPressed(Vortex::Key::K)) {
			m_SquarePosition.y -= m_SquareMoveSpeed * float(ts);
		}


		// Make the object rotate left
		Vortex::RenderCommand::SetClearColor({0, 0, 0, 1});
		Vortex::RenderCommand::ClearBufferBit();

		// Update event
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Vortex::Renderer::BeginScene(m_Camera);
		Vortex::Renderer::Submit(m_Shader, m_VertexArray, glm::mat4(1.0f));
		
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
		Vortex::Renderer::EndScene();
	}

	void OnEvent(Vortex::Event &event) override 
	{
		

	}

private:
	// Buffer and Shader
	std::shared_ptr<Vortex::Shader> m_Shader;
	std::shared_ptr<Vortex::VertexArray> m_VertexArray;
	
	std::shared_ptr<Vortex::Shader> m_ShaderSquare;
	std::shared_ptr<Vortex::VertexArray> m_VertexArraySquare;

	// Camera component
	Vortex::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
	float m_CameraSpeed = 1.0f;
	float m_CameraRotation = 0.1f;
	float m_CameraRotationSpeed = 30.0f;

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