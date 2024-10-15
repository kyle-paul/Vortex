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

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnImGuiRender() override;

        void Begin();
        void End();

    private:
        static void setKeyMapping(); 
        static void configColorStyle();
        
    private:
        float m_time = 0.0f;
    };
}