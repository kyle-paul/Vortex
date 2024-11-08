#include "Vortex/Core/Core.h"
#include "Vortex/Core/Application.h"

#define BIND_EVENT_FUNCTION(x) std::bind(&x, this, std::placeholders::_1)

namespace Vortex 
{
	Application *Application::m_AppInstance = nullptr;

	Application::Application(const std::string &name) 
	{
		VX_PROFILE_FUNCTION();

		// Only one window
		VX_CORE_ASSERT(!m_AppInstance, "Application already exits!");
		m_AppInstance = this;

		// Init window
		WindowProps props(name, 2100, 1200);
		m_AppWindow = std::unique_ptr<Window>(Window::Create(props));
		m_AppWindow->SetEventCallback( BIND_EVENT_FUNCTION(Application::OnEvent) );

		// Init Renderer
		Renderer::Init();

		// Init default Imgui Layer
		m_ImGuiLayer = new ImGuiLayer();
		PushOverLay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		VX_PROFILE_FUNCTION();
	}

	void Application::Close()
	{
		m_IsRunning = false;
	}

	void Application::OnEvent(Event &event) {

		VX_PROFILE_FUNCTION();
		
		// Close window event
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>( BIND_EVENT_FUNCTION(Application::OnWindowClose) );
		dispatcher.Dispatch<WindowResizeEvent>( BIND_EVENT_FUNCTION(Application::OnWindowResize) );

		// Layer check
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (event.Handled) 
				break;
			(*it)->OnEvent(event);
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent &event) {		
		m_IsRunning = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		VX_PROFILE_FUNCTION();
		
		if (event.GetWidth() == 0 || event.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());
		return false;
	}

	void Application::PushLayer(Layer *layer) {
		VX_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	
	void Application::PushOverLay(Layer *overlayer) {
		VX_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(overlayer);
		overlayer->OnAttach();
	}

	void Application::Run() 
	{
		VX_PROFILE_FUNCTION();
				
		while(m_IsRunning) 
		{
			// Timesteps and delta time
			float time = (float)glfwGetTime(); // -> Platform::GetTime()
			TimeStep m_TimeStep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			
			if (!m_Minimized)
			{
				// Update Layers (except ImGui default layer)
				{
					VX_PROFILE_SCOPE("LayerStack OnUpdate");
					for (Layer *layer: m_LayerStack) { layer->OnUpdate(m_TimeStep); }
				}
			}

			// Update all windows in ImGui layers
			m_ImGuiLayer->Begin();
			{
				VX_PROFILE_SCOPE("LayerStack OnImGuiRender");
				for (Layer *layer: m_LayerStack) { layer->OnImGuiRender(); }
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