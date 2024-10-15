#include "Vortex/Core/Core.h"
#include "Vortex/PreHeaders.h"
#include "Vortex/Core/Application.h"
#include "Vortex/Events/ApplicationEvent.h"
#include "Vortex/Core/Logging.h"

#include <iostream>
#include <memory>

#define BIND_EVENT_FUNCTION(x) std::bind(&x, this, std::placeholders::_1)

namespace Vortex 
{
	Application::Application()
	{
		WindowProps props("Vortex Engine", 1000, 650);
		m_appWindow = std::unique_ptr<Window>(Window::Create(props));
		m_appWindow->SetEventCallback( BIND_EVENT_FUNCTION(Application::OnEvent) );
	}

	Application::~Application()
	{
		
	}

	void Application::OnEvent(Event &event) {
		VX_CORE_INFO("{0}", event.ToString());
	}

	void Application::Run() {
		WindowResizeEvent e(1000, 650);
		
		while(m_IsRunning) {
			glClearColor(0.5, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_appWindow->OnUpdate();
		}
	}
}
