#pragma once

#include "Vortex/PreHeaders.h"
#include "Vortex/Core/Core.h"
#include "Vortex/Core/Logging.h"
#include "Vortex/Core/Assert.h"
#include "Vortex/Core/Window.h"
#include "Vortex/Core/Layer.h"
#include "Vortex/Core/LayerStack.h"
#include "Vortex/Core/Input.h"
#include "Vortex/Core/Math.h"
#include "Vortex/Core/TimeStep.h"

#include "Vortex/Events/ApplicationEvent.h"
#include "Platform/Linux/LinuxWindow.h"
#include "Vortex/Imgui/ImGuiLayer.h"

#include "Graphics/Shader.h"
#include "Graphics/Buffer.h"
#include "Graphics/VertexArray.h"
#include "Graphics/Renderer.h"
#include "Graphics/OrthographicCamera.h"

#include <GLFW/glfw3.h>

namespace Vortex {
	class VORTEX_API Application 
	{
	public: 
		Application(const std::string &name = "Vortex Application");
		virtual ~Application();
		void Run();
		void OnEvent(Event &event);
		void PushLayer(Layer *layer);
		void PushOverLay(Layer *overlay);

		inline static Application& GetApplication() { return *m_AppInstance; } 
		inline Window& GetWindow() { return *m_AppWindow; }
		inline ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		void Close();
	
	private:
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

	private:
		bool m_IsRunning = true;
		bool m_Minimized = false;
		
		// Window and Layers
		std::unique_ptr<Window> m_AppWindow;
		ImGuiLayer* m_ImGuiLayer;
		static Application *m_AppInstance;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;
	};

	// To be defined in client
	Application* CreateApplication();
}