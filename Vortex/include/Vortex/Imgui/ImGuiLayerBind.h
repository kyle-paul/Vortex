#pragma once

#include "Vortex/Core/Layer.h"
#include "Vortex/Core/TimeStep.h"

#include "Vortex/Events/MouseEvent.h"
#include "Vortex/Events/ApplicationEvent.h"
#include "Vortex/Events/KeyEvent.h"

namespace Vortex 
{
    class VORTEX_API ImGuiLayerBind : public Layer 
    {
    public:
        ImGuiLayerBind();
        ~ImGuiLayerBind();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(TimeStep ts) override;
        virtual void OnImGuiRender() override;
        
        void OnEvent(Event &event);
        void Begin();
        void End();

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