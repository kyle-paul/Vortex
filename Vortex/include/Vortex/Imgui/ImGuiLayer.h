#pragma once

#include "Vortex/Core/Layer.h"
#include "Vortex/Events/MouseEvent.h"
#include "Vortex/Events/ApplicationEvent.h"
#include "Vortex/Events/KeyEvent.h"

namespace Vortex 
{
    class VORTEX_API ImGuiLayer : public Layer 
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnUpdate();
        void OnAttach() override;
        void OnEvent(Event &event);

    private:
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent &event);
        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent &event);
        bool OnMouseMovedEvent(MouseMovedEvent &event);
        bool OnMouseScrolledEvent(MouseScrolledEvent &event);

        bool OnWindowResizeEvent(WindowResizeEvent &event);
        bool OnKeyPressedEvent(KeyPressedEvent &event);
        bool OnKeyReleasedEvent(KeyReleasedEvent &event);
        bool OnKeyTypedEvent(KeyTypedEvent &event);

        static void setKeyMapping(); 
        static void configColorStyle();

    private:
        float m_time = 0.0f;
    };
}