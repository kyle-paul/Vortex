#pragma once

#include "Vortex/Core/Core.h"
#include "Vortex/PreHeaders.h"
#include "Vortex/Events/Event.h"
#include "Vortex/Core/TimeStep.h"

namespace Vortex {
    class VORTEX_API Layer {
    public:
        Layer(const std::string &debugName = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {};
        virtual void OnDetach() {};
        virtual void OnUpdate(TimeStep ts) {};
        virtual void OnImGuiRender() {};
        virtual void OnEvent(Event &event) {};
        inline const std::string& GetName() const { return m_DebugName; } 
    private:
        std::string m_DebugName;
    };
}