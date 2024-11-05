#include "Vortex/Core/WindowInput.h"
#include "Vortex/PreHeaders.h"
#include "Vortex/Core/Application.h"

#include "GLFW/glfw3.h"

namespace Vortex 
{
    // create the derive instance class with defined override functions 
    Input *Input::m_InputInstance = new WindowInput(); 

    bool WindowInput::IsKeyPressedImpl(uint16_t keycode) {
        auto window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WindowInput::IsMouseButtonPressedImpl(uint16_t button) {
        auto window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> WindowInput::GetMousePosImpl() {
        auto window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        return { (float)xPos, (float)yPos };
    }

    float WindowInput::GetMouseXImpl() {
        auto [x,y] = GetMousePosImpl();
        return x;
    }

    float WindowInput::GetMouseYImpl() {
        auto [x,y] = GetMousePosImpl();
        return y;
    }
}