#pragma once
#include "Vortex.h"
#include <imgui_node_editor.h>

namespace Vortex
{
    class NodeEditor
    {
    public:
        NodeEditor();
        ~NodeEditor();
        void OnAttach();
        void OnDetach();
        void OnImGuiRender();
    };
}