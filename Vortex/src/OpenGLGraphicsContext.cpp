#include "Platform/OpenGL/OpenGLGraphicsContext.h"

namespace Vortex
{
    OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow *windowHandle)
        : m_WindowHandle(windowHandle) 
    {
        VX_CORE_ASSERT(m_WindowHandle, "Window Handle is null!")
    }
    
    OpenGLGraphicsContext::~OpenGLGraphicsContext()
    {

    }

    void OpenGLGraphicsContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		VX_CORE_ASSERT(status, "Failed to initialize GLAD with GLFW (OPENGL)");
    }

    void OpenGLGraphicsContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}