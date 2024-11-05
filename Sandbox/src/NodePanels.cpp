#include "NodePanels.h"

namespace Vortex
{
    NodePanels::NodePanels()
    {
        
    }

    NodePanels::~NodePanels()
    {

    }

    void NodePanels::OnAttach()
    {
        ImNodes::CreateContext();
    }

    void NodePanels::OnDetach()
    {
        ImNodes::DestroyContext();
    }

    void NodePanels::OnImGuiRender()
    {
        ImGui::Begin("Control Flow");

        ImNodes::BeginNodeEditor();

        // First Node
        const int node_id_1 = 1;
        ImNodes::BeginNode(node_id_1);
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("First Gate");
        ImNodes::EndNodeTitleBar();
        ImGui::Dummy(ImVec2(80.0f, 45.0f));

        const int output_attr_id_1 = 2;
        ImNodes::BeginOutputAttribute(output_attr_id_1);
        ImGui::Text("output gate 1");
        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();

        // Second Node
        const int node_id_2 = 3;
        ImNodes::BeginNode(node_id_2);
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Second Gate");
        ImNodes::EndNodeTitleBar();
        ImGui::Dummy(ImVec2(100.0f, 60.0f));

        const int output_attr_id_2 = 4;
        ImNodes::BeginOutputAttribute(output_attr_id_2);
        ImGui::Text("output gate 2");
        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();

        ImNodes::MiniMap();
        ImNodes::EndNodeEditor();

        ImGui::End();
    }
}