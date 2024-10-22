#pragma once
#include "Vortex/PreHeaders.h"
#include "Vortex/Core/Core.h"
#include "Vortex/Core/Assert.h"
#include "Vortex/Core/Window.h"

#include "Vortex/Events/ApplicationEvent.h"
#include "Vortex/Events/MouseEvent.h"
#include "Vortex/Events/KeyEvent.h"

#include "Graphics/GraphicsContext.h"
#include "Platform/OpenGL/OpenGLGraphicsContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Vortex 
{
	class LinuxWindow : public Window
	{
	public:
		LinuxWindow(const WindowProps& props);
		~LinuxWindow();

		void OnUpdate() override;

		uint32_t GetWidth() const override { return m_WindowConfig.Width; }
		uint32_t GetHeight() const override { return m_WindowConfig.Height; }
		
		inline virtual void* GetNativeWindow() const { return m_glfw_Window; }

		// Window attribute
		inline void SetEventCallback(const EventCallbackFn& callback) override { 
			m_WindowConfig.EventCallback = callback; 
		}
		
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:
		void Init(const WindowProps& props);
		void Shutdown();

	private:
		GLFWwindow* m_glfw_Window;
		GraphicsContext* m_context;

		int s_GLFWWindowCount = 0;

		struct WindowConfig
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;
		};

		WindowConfig m_WindowConfig;
	};
}