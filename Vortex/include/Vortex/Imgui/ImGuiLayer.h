#pragma once

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
    struct ImGuiComponents {
        glm::vec4 ObjectColor = {0.8f, 0.2f, 0.2f, 1.f};
        glm::vec3 ObjectPosition = {0.0f, 0.0f, 1.0f};
        glm::vec2 ObjectSize = {1.0f, 1.0f};
        float ObjectRotation = 10.0f;

        glm::vec4 BoardColor = {0.2f, 0.2f, 0.8f, 0.5f};
        float BoardRotation = 10.0f;
        float TilingFactor = 1.0f;
    };

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