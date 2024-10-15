#pragma once

#include "Vortex/Core/Layer.h"

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
        float m_time = 0.0f;
    };
}