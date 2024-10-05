#pragma once

namespace Hazel {
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