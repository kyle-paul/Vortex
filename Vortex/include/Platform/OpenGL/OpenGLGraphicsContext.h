#pragma once
#include <glad/glad.h>
#include "Graphics/GraphicsContext.h"
#include "GLFW/glfw3.h"
#include "Vortex/Core/Assert.h"
#include <iostream>

namespace Vortex 
{
    class OpenGLGraphicsContext : public GraphicsContext
    {
    public:
        OpenGLGraphicsContext(GLFWwindow *windowHandle);
        ~OpenGLGraphicsContext();

        virtual void Init() override;
        virtual void SwapBuffers() override;
    private:
        GLFWwindow *m_WindowHandle;
    };
}