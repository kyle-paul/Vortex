#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

#include "Vortex/PreHeaders.h"
#include "Vortex/Core/Layer.h"
#include "Vortex/Core/TimeStep.h"
#include "Vortex/Core/Math.h"

#include "Vortex/Events/MouseEvent.h"
#include "Vortex/Events/ApplicationEvent.h"
#include "Vortex/Events/KeyEvent.h"

#include "Vortex/Imgui/ImGuiGlfwRender.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

#include "imgui.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Vortex 
{
    class VORTEX_API ImGuiLayer : public Layer 
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event &event) override;
        virtual void OnUpdate(TimeStep ts) override;

        void Begin();
        void End();

        void SetBlockEvent(bool block) { m_BlockEvent = block; }

    private:
        static void setKeyMapping(); 
        static void configColorStyle();
        
    private:
        float m_time = 0.0f;
        bool dockspaceOpen = true;
        bool fullscreen = true;
        bool show_demo_window = true;
        bool show_another_window = true;
        bool m_BlockEvent = true;
    };
}