#include "NodeEditorWidget.h"
#include "VolumeNodeWidget.h"
namespace ui {

NodeEditorWidget::NodeEditorWidget() {
    this->nodes.insert({0,std::unique_ptr<NodeWidget>(new VolumeNodeWidget())});
}

NodeEditorWidget::~NodeEditorWidget() {}

void NodeEditorWidget::render() {

    ImGui::Begin(this->_name.c_str());
    static bool sel = false;


    ImNodes::BeginNodeEditor();

    for(const auto &it : nodes) {
        it.second->render();
    }

    // check for right mouse
    bool open_popup = false;
    if (ImGui::GetIO().MouseClicked[1]) {
        open_popup = true;
    }

    // context menu
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 8.f));
    if (!ImGui::IsAnyItemHovered() && open_popup)
    {
        ImGui::OpenPopup("add node");
    }
    // context menu items
    if (ImGui::BeginPopup("add node"))
    {
        if (ImGui::MenuItem("add"))
        {
        }

        if (ImGui::MenuItem("sine"))
        {
        }

        if (ImGui::MenuItem("time"))
        {
        }

        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();

    // demo nodes
    ImNodes::BeginNode(1);
    ImNodes::BeginOutputAttribute(1);
    if(sel) {
        ImGui::Text("selected");
    }
    else {
        ImGui::Text("output pin");
    }

    ImNodes::EndOutputAttribute();
    ImNodes::EndNode();

    ImNodes::BeginNode(2);
    ImNodes::BeginInputAttribute(2);
    ImGui::Text("input pin");
    ImNodes::EndInputAttribute();
    ImNodes::EndNode();

    // render links
    for (unsigned int i = 0; i < links.size(); ++i)
    {
        const std::pair<int, int> p = links[i];
        ImNodes::Link(i, p.first, p.second);
    } 
    ImNodes::EndNodeEditor();

    // check for connections
    int start_attr, end_attr;
    if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
    {
        links.push_back(std::make_pair(start_attr, end_attr));
    }

    // get selected nodes
    int num_selected_nodes = ImNodes::NumSelectedNodes();
    if (num_selected_nodes > 0)
    {
        std::vector<int> selected_nodes;
        selected_nodes.resize(num_selected_nodes);
        ImNodes::GetSelectedNodes(selected_nodes.data());
        if (selected_nodes[0] == 1) {
            sel = true;
        }
        else {
            sel = false;
        }
    }

    ImGui::End();

}

}