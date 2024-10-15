#include "Platform/Linux/LinuxWindow.h"

#include "Vortex/PreHeaders.h"
#include "Vortex/Events/ApplicationEvent.h"
#include "Vortex/Events/MouseEvent.h"
#include "Vortex/Events/KeyEvent.h"
#include "Vortex/Core/Assert.h"

namespace Vortex 
{
	static bool s_GLFWInitialized = false;
	static void GLFW_ErrorCallBack(int error_code, const char* description) {
		VX_CORE_ERROR("GLFW Error ({0}): {1}", error_code, description);
	}

	Window* Window::Create(const WindowProps& props) {
		return new LinuxWindow(props);
	}

	LinuxWindow::LinuxWindow(const WindowProps& props) {
		Init(props);
	}

	LinuxWindow::~LinuxWindow() {
		
	}

	void LinuxWindow::Init(const WindowProps& props) {
		m_WindowConfig.Title = props.Title;
		m_WindowConfig.Width = props.Width;
		m_WindowConfig.Height = props.Height;

		VX_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized) {
			int success = glfwInit();
			VX_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFW_ErrorCallBack);
			s_GLFWInitialized = true;
		}

		m_glfw_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_WindowConfig.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_glfw_Window);
		glfwSetWindowUserPointer(m_glfw_Window, &m_WindowConfig);
		SetVSync(true);

		// Set GLFW callback
		// GLFW resize callback with lambda function
		glfwSetWindowSizeCallback(m_glfw_Window, [](GLFWwindow* window, int width, int height)
		{
			std::cout << "Window resized: " << width << "x" << height << std::endl;
			WindowConfig &t_windowConfig = *(WindowConfig*)glfwGetWindowUserPointer(window);
			WindowResizeEvent event(width, height);
			t_windowConfig.EventCallback(event);
			t_windowConfig.Width = width;
			t_windowConfig.Height = height;
		});

		// GLFW close callback with lambda function
		glfwSetWindowCloseCallback(m_glfw_Window, [](GLFWwindow* window){
			WindowConfig &t_windowConfig = *(WindowConfig*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
		});

		// GLFW set key callback
		glfwSetKeyCallback(m_glfw_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
			WindowConfig &t_windowConfig = *(WindowConfig*)glfwGetWindowUserPointer(window);
			switch(action) {
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					t_windowConfig.EventCallback(event);
					break;
				}
					
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					t_windowConfig.EventCallback(event);
					break;
				}
					
				case GLFW_REPEAT: 
				{
					KeyPressedEvent event(key, 1);
					t_windowConfig.EventCallback(event);
					break;
				}
			}
		});

		// GLFW mouse button callback
		glfwSetMouseButtonCallback(m_glfw_Window, [](GLFWwindow* window, int button, int action, int mods){
			WindowConfig &t_windowConfig = *(WindowConfig*)glfwGetWindowUserPointer(window);
			switch(action) {
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					t_windowConfig.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					t_windowConfig.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_glfw_Window, [](GLFWwindow* window, double xoffset, double yoffset){
			WindowConfig &t_windowConfig = *(WindowConfig*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xoffset, (float)yoffset);
			t_windowConfig.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_glfw_Window, [](GLFWwindow* window, double xpos, double ypos){
			WindowConfig &t_windowConfig = *(WindowConfig*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event(xpos, ypos);
			t_windowConfig.EventCallback(event);
		});
	}

	void LinuxWindow::Shutdown() {
		glfwDestroyWindow(m_glfw_Window);
	}

	void LinuxWindow::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(m_glfw_Window);
	}

	void LinuxWindow::SetVSync(bool enabled) {
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_WindowConfig.VSync = enabled;
	}

	bool LinuxWindow::IsVSync() const {
		return m_WindowConfig.VSync;	
	}
}