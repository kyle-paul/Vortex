#pragma once

#include "Vortex/PreHeaders.h"
#include "Vortex/Events/ApplicationEvent.h"
#include "Vortex/Core/Logging.h"

#include <sstream>

namespace Vortex {
	class Application 
	{
	public: 
		Application();
		virtual ~Application();
		void Run();
	};

	// To be defined in client
	Application* CreateApplication();
}