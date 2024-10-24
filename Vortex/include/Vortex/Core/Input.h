#pragma once

#include "Vortex/Core/Core.h"

namespace Vortex 
{
    class VORTEX_API Input
    {
    public:
        virtual ~Input() = default;

        static bool IsKeyPressed(uint16_t keycode) { return m_InputInstance->IsKeyPressedImpl(keycode); }
        static bool IsMouseButtonPressed(uint16_t button) { return m_InputInstance->IsMouseButtonPressedImpl(button); }

        static std::pair<float, float> GetMousePos() { return m_InputInstance->GetMousePosImpl(); }

        static bool GetMouseX() { return m_InputInstance->GetMouseXImpl(); }
        static bool GetMouseY() { return m_InputInstance->GetMouseYImpl(); }

    protected:
        virtual bool IsKeyPressedImpl(uint16_t keycode) = 0;
        virtual bool IsMouseButtonPressedImpl(uint16_t keycode) = 0;
        
        virtual std::pair<float, float> GetMousePosImpl() = 0;
        
        virtual float GetMouseXImpl() = 0;
        virtual float GetMouseYImpl() = 0;

    private:
        static Input* m_InputInstance;
    };
}