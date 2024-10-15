#pragma once

#include "Vortex/Core/Core.h"
#include "Vortex/PreHeaders.h"
#include "Vortex/Events/ApplicationEvent.h"
#include "Vortex/Core/Logging.h"
#include "Vortex/Core/Window.h"
#include "Platform/Linux/LinuxWindow.h"
#include <GLFW/glfw3.h>
#include <sstream>

namespace Vortex {
	class VORTEX_API Application 
	{
	public: 
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event &event);

	private:
		std::unique_ptr<Window> m_appWindow;
		bool m_IsRunning = true;
	};

	// To be defined in client
	Application* CreateApplication();
}