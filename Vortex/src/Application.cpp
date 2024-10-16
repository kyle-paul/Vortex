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

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float verts[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f,
		};
		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		unsigned int faces[3] = { 0, 1, 2 };
		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(faces), faces, GL_STATIC_DRAW);

		std::string vertexSource = getVertexSource();
		std::string fragmentSource = getFragmentSource();
		m_Shader = std::make_unique<Shader>(vertexSource, fragmentSource);
	}

	Application::~Application()
	{
		
	}

	std::string Application::getVertexSource() {
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
	
	std::string Application::getFragmentSource() {
		return R"(
			#version 330 core
			
			layout(location=0) out vec4 color;
			in vec3 v_Position;

			void main() {
				color = vec4(v_Position, 1.0);
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

			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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