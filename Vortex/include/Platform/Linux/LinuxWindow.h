#pragma once
#include "Vortex/Core/Core.h"
#include "Vortex/Core/Window.h"
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