#pragma once
#include "Vortex.h"
#include <imnodes.h>

namespace Vortex
{
    struct Node
    {
        int   id;
        float value;

        Node(const int i, const float v) : id(i), value(v) {}
    };

    struct Link
    {
        int id;
        int start_attr, end_attr;
    };


    struct Editor
    {
        ImNodesEditorContext* context = nullptr;
        std::vector<Node>     nodes;
        std::vector<Link>     links;
        int                   current_id = 0;
    };


    class NodePanels
    {
    public:
        NodePanels();
        ~NodePanels();

        void OnAttach();
        void OnDetach();
        void OnImGuiRender();
    };
}