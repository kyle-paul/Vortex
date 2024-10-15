#include "Vortex/Core/Core.h"
#include "Vortex/Core/Input.h"

namespace Vortex {
    class VORTEX_API WindowInput : public Input
    {
    public:

    protected:
        virtual bool IsKeyPressedImpl(uint16_t keycode) override;
        virtual bool IsMouseButtonPressedImpl(uint16_t button) override;
        virtual std::pair<float,float> GetMousePosImpl() override;
        virtual float GetMouseXImpl() override;
        virtual float GetMouseYImpl() override;
    };
}