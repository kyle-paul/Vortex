#include "Vortex/Core/Application.h"

#define BIND_EVENT_FUNCTION(x) std::bind(&x, this, std::placeholders::_1)

namespace Vortex 
{
	Application *Application::m_AppInstance = nullptr;

	Application::Application()
	{
		// Only one window
		VX_CORE_ASSERT(!m_AppInstance, "Application already exits!");
		m_AppInstance = this;

		// Init window
		WindowProps props("Vortex Engine", 1300, 800);
		m_AppWindow = std::unique_ptr<Window>(Window::Create(props));
		m_AppWindow->SetEventCallback( BIND_EVENT_FUNCTION(Application::OnEvent) );

		// Init default Imgui Layer
		m_ImGuiLayer = new ImGuiLayer();
		PushOverLay(m_ImGuiLayer);

		// ================================ Graphics API ================================
		// ================================ Object 1 ================================

		// 1. Vertex Array
		m_VertexArray.reset(VertexArray::Create());
		
		// 2.1 Vertex Buffer
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f,  0.2f, 0.3f, 0.8f, 1.0f,
			0.0f, 0.5f, 0.0f,   0.8f, 0.8f, 0.2f, 1.0f 
		};

		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		
		// 2.2 Layout Buffer
		m_VertexBuffer->SetLayout({
			{  ShaderDataType::Float3, "a_Position" },
			{  ShaderDataType::Float4, "a_Color"},
		});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		// 3. Index BUffer
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(IndexBuffer::Create(indices, 3));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		// 4. Shader program
		std::string vertexSource = getVertexSource();
		std::string fragmentSource = getFragmentSource();
		m_Shader = std::make_unique<Shader>(vertexSource, fragmentSource);


		// ================================ Object 2 ================================
		m_VertexArraySquare.reset(VertexArray::Create());
		
		float verticesSquare[4 * 3] = {
			-0.55f, -0.55, 0.0f,
			0.55f, -0.55f, 0.0f,
			0.55f, 0.55f, 0.0f,
			-0.55f, 0.55f, 0.0f
		};

		std::shared_ptr<VertexBuffer> m_VertexBufferSquare;
		m_VertexBufferSquare.reset(VertexBuffer::Create(verticesSquare, sizeof(verticesSquare)));

		m_VertexBufferSquare->SetLayout({
			{  ShaderDataType::Float3, "a_Position" },
		});
		m_VertexArraySquare->AddVertexBuffer(m_VertexBufferSquare);

		uint32_t indicesSquare[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> m_IndexBufferSquare;
		m_IndexBufferSquare.reset(IndexBuffer::Create(indicesSquare, 6));
		m_VertexArraySquare->SetIndexBuffer(m_IndexBufferSquare);

		std::string vertexSourceSquare = getVertexSourceSquare();
		std::string fragmentSourceSquare = getFragmentSourceSquare();
		m_ShaderSquare = std::make_unique<Shader>(vertexSourceSquare, fragmentSourceSquare);
	}

	Application::~Application()
	{
		
	}

	std::string Application::getVertexSource() {
		return R"(
			#version 330 core
			
			layout(location=0) in vec3 a_Position;
			layout(location=1) in vec4 a_Color;
			out vec3 v_Position;
			out vec4 v_Color;
			
			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position + 0.3, 1.0);
			}
		)";
	}
	
	std::string Application::getFragmentSource() {
		return R"(
			#version 330 core
			
			in vec3 v_Position;
			in vec4 v_Color;
			layout(location=0) out vec4 color;

			void main() {
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";
	}

	std::string Application::getVertexSourceSquare() {
		return R"(
			#version 330 core
			
			layout(location=0) in vec3 a_Position;
			out vec3 v_Position;
			
			void main() {
				v_Position = a_Position;
				gl_Position = vec4(a_Position + 0.3, 1.0);
			}
		)";
	}
	
	std::string Application::getFragmentSourceSquare() {
		return R"(
			#version 330 core
			
			in vec3 v_Position;
			layout(location=0) out vec4 color;

			void main() {
				color = vec4(0.0, 0.0, 0.5, 1.0);
			}
		)";
	}

	bool Application::OnWindowClose(WindowCloseEvent &event) {
		m_IsRunning = false;
		return true;
	}

	void Application::OnEvent(Event &event) {
		
		// Close window event
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>( BIND_EVENT_FUNCTION(Application::OnWindowClose) );

		// Layer check
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (event.Handled) 
				break;
			(*it)->OnEvent(event);
		}
	}

	void Application::PushLayer(Layer *layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	
	void Application::PushOverLay(Layer *overlayer) {
		m_LayerStack.PushOverlay(overlayer);
		overlayer->OnAttach();
	}

	void Application::Run() {
		WindowResizeEvent e(1000, 650);
		
		while(m_IsRunning) {
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_ShaderSquare->Bind();
			m_VertexArraySquare->Bind();
			glDrawElements(GL_TRIANGLES, m_VertexArraySquare->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			m_Shader->Bind();
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			// Update Layers (except ImGui default layer)
			for (Layer *layer: m_LayerStack) {
				layer->OnUpdate();
			}

			// Update all windows in ImGui layers
			m_ImGuiLayer->Begin();
			for (Layer *layer: m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();
			m_AppWindow->OnUpdate();
		}
	}
}



// In the running loop
// && !glfwWindowShouldClose(static_cast<GLFWwindow*>(m_AppWindow->GetNativeWindow()))
// auto [x,y] = Input::GetMousePos();
// VX_CORE_TRACE(" {0} - {1} ", x, y);