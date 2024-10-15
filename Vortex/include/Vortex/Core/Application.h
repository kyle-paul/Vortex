#pragma once

#include "Vortex/Core/Core.h"
#include "Vortex/PreHeaders.h"
#include "Vortex/Events/ApplicationEvent.h"
#include "Vortex/Core/Logging.h"
#include "Vortex/Core/Window.h"
#include "Vortex/Core/Layer.h"
#include "Vortex/Core/LayerStack.h"
#include "Platform/Linux/LinuxWindow.h"
#include "Vortex/Imgui/ImGuiLayerBind.h"
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
		void PushLayer(Layer *layer);
		void PushOverLay(Layer *overlay);

		inline static Application& GetApplication() { return *m_AppInstance; } 
		inline Window& GetWindow() { return *m_AppWindow; }

	private:
		bool OnWindowClose(WindowCloseEvent &event);
		
		std::unique_ptr<Window> m_AppWindow;
		std::unique_ptr<ImGuiLayerBind> m_ImGuiLayer;

		bool m_IsRunning = true;
		LayerStack m_LayerStack;

		static Application *m_AppInstance;
	};

	// To be defined in client
	Application* CreateApplication();
}