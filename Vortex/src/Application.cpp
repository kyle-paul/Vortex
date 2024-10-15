#include "Vortex/Core/Core.h"
#include "Vortex/PreHeaders.h"
#include "Vortex/Core/Assert.h"
#include "Vortex/Core/Application.h"
#include "Vortex/Events/ApplicationEvent.h"
#include "Vortex/Core/Logging.h"
#include "Vortex/Core/Input.h"

#include "Vortex/Core/Math.h"

#include <iostream>
#include <memory>

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
		WindowProps props("Vortex Engine", 1300, 700);
		m_AppWindow = std::unique_ptr<Window>(Window::Create(props));
		m_AppWindow->SetEventCallback( BIND_EVENT_FUNCTION(Application::OnEvent) );

		// Init default Imgui Layer
		m_ImGuiLayer = std::make_unique<ImGuiLayerBind>();
		PushOverLay(m_ImGuiLayer.get());
	}

	Application::~Application()
	{
		
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
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(event);
			if(event.Handled) {
				break;
			}
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
		
		while(m_IsRunning && !glfwWindowShouldClose(static_cast<GLFWwindow*>(m_AppWindow->GetNativeWindow()))) {
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

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

			// Get combo input
			// auto [x,y] = Input::GetMousePos();
 			// VX_CORE_TRACE(" {0} - {1} ", x, y);

			m_AppWindow->OnUpdate();
		}
	}
}
