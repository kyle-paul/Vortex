#include "Vortex/Core/Application.h"
#include "Vortex/Events/ApplicationEvent.h"
#include "Vortex/Core/Logging.h"

#include <iostream>
#include <memory>

namespace Vortex 
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
		
	}

	void Application::Run() {
		WindowResizeEvent e(1280, 720);
		
		while(m_IsRunning) {
			m_Window->OnUpdate();
		}
	}
}
