#include "Vortex/PreHeaders.h"
#include "Vortex/Imgui/ImGuiLayer.h"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "Vortex/Core/Application.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define VX_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Vortex 
{
    ImGuiLayer::ImGuiLayer() {

    }

    ImGuiLayer::~ImGuiLayer() {

    }

    void ImGuiLayer::OnUpdate() {
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        Application &app = Application::GetApplication();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        float time = (float)glfwGetTime();
        io.DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.0f);
        m_time = time;
        
        bool show_demo_window = true;
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::OnAttach() {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        const char* glsl_version = "#version 410";
        ImGui_ImplOpenGL3_Init(glsl_version);

        ImGuiIO& io = ImGui::GetIO(); 
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        
        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE; // This is the missing key
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A; // For Ctrl+A (select all)
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C; // For Ctrl+C (copy)
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V; // For Ctrl+V (paste)
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X; // For Ctrl+X (cut)
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y; // For Ctrl+Y (redo)
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z; // For Ctrl+Z (undo)
    }

    void ImGuiLayer::OnEvent(Event &event) {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseButtonPressedEvent>(VX_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(VX_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(VX_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(VX_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));

        dispatcher.Dispatch<WindowResizeEvent>(VX_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
        dispatcher.Dispatch<KeyPressedEvent>(VX_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
        dispatcher.Dispatch<KeyReleasedEvent>(VX_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
        dispatcher.Dispatch<KeyTypedEvent>(VX_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
    }

    bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent &event) {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[event.GetMouseButton()] = true;
        return false;
    }

    bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent &event) {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[event.GetMouseButton()] = false;
        return false;
    }

    bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent &event) {
        ImGuiIO &io = ImGui::GetIO();
        io.MousePos = ImVec2(event.GetX(), event.GetY());
        return false;
    }

    bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent &event) {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseWheelH += event.GetXOffset();
        io.MouseWheel += event.GetYOffset();
        return false;
    }


    bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent &event) {
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2(event.GetWidth(), event.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, event.GetWidth(), event.GetHeight());
        return false;
    }


    bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent &event) {
        ImGuiIO &io = ImGui::GetIO();
        io.KeysDown[event.GetKeyCode()] = true;

        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
        return false;
    }

    bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent &event) {
        ImGuiIO &io = ImGui::GetIO();
        io.KeysDown[event.GetKeyCode()] = false;
        return false;
    }

    bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent &event) {
        ImGuiIO &io = ImGui::GetIO();
        uint16_t keycode = event.GetKeyCode();

        if (keycode > 0 && keycode < 0x10000) {
            io.AddInputCharacter((unsigned short)keycode);
        }
        return false;
    }
}