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

#include "Vortex/Events/ApplicationEvent.h"
#include "Platform/Linux/LinuxWindow.h"
#include "Vortex/Imgui/ImGuiLayer.h"

#include "Graphics/Shader.h"
#include "Graphics/Buffer.h"
#include "Graphics/VertexArray.h"
#include "Graphics/Renderer.h"

#include <GLFW/glfw3.h>

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
		std::string getVertexSource();
		std::string getFragmentSource();

		std::string getVertexSourceSquare();
		std::string getFragmentSourceSquare();

	private:
		bool m_IsRunning = true;
		bool OnWindowClose(WindowCloseEvent &event);
		
		std::unique_ptr<Window> m_AppWindow;
		ImGuiLayer* m_ImGuiLayer;
		static Application *m_AppInstance;
		LayerStack m_LayerStack;

		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<VertexArray> m_VertexArraySquare;
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<Shader> m_ShaderSquare;
	};

	// To be defined in client
	Application* CreateApplication();
}