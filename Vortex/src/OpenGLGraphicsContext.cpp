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
        
        const GLubyte* vendor = glGetString(GL_VENDOR);
        const GLubyte* device = glGetString(GL_RENDERER);
        const GLubyte* version = glGetString(GL_VERSION);
        VX_CORE_INFO("OpenGL Version: {0}", reinterpret_cast<const char*>(version));
        VX_CORE_INFO("OpenGL Rendering Device is: {0} - {1}", 
                    reinterpret_cast<const char*>(vendor), 
                    reinterpret_cast<const char*>(device));

        // Check if the vendor is NVIDIA
        if (vendor && strstr(reinterpret_cast<const char*>(vendor), "NVIDIA") != nullptr) {
            VX_CORE_INFO("NVIDIA Graphics Card Detected");
        } else {
            VX_CORE_INFO("Using Default Graphics Card");
        }
    }

    void OpenGLGraphicsContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}