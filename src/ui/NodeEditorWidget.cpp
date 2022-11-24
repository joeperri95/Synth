#include "NodeEditorWidget.h"
#include "VolumeNodeWidget.h"
#include "SourceNodeWidget.h"
#include "SinkNodeWidget.h"
#include <iostream>

namespace ui {

NodeEditorWidget::NodeEditorWidget() {
    this->controller = std::make_shared<PipelineController>();
}


NodeEditorWidget::~NodeEditorWidget() {}

void NodeEditorWidget::render() {

    ImGui::Begin(this->_name.c_str());

    ImNodes::BeginNodeEditor();

    std::vector<std::shared_ptr<NodeWidget>> nodes = controller->getNodes();

    for(const auto &it : nodes) {
        it->render();
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
        if (ImGui::MenuItem("volume"))
        {
            controller->addNode("volume");
        }

        if (ImGui::MenuItem("source"))
        {
            controller->addNode("source");
        }

        if (ImGui::MenuItem("sink"))
        {
            controller->addNode("sink");
        }

        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();

    // render links
    
    std::map<LinkID, std::pair<int, int>> links = controller->getLinks();
    for (const auto &[linkid, pair]: links)
    {
        ImNodes::Link(linkid, pair.first, pair.second);
    }
    

    ImNodes::EndNodeEditor();

    // check for connections
    int start_attr, end_attr;
    if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
    {
        controller->addLink(start_attr, end_attr);
    }

    // check for removed connections
    int out_attr;
    if (ImNodes::IsLinkDestroyed(&out_attr))
    {
        controller->removeLink(out_attr);
    }

    // get selected nodes
    int num_selected_nodes = ImNodes::NumSelectedNodes();
    if (num_selected_nodes > 0)
    {
        std::vector<int> selected_nodes;
        selected_nodes.resize(num_selected_nodes);
        ImNodes::GetSelectedNodes(selected_nodes.data());

        controller->selectNodes(selected_nodes);
    }
    else 
    {
        std::vector<int> selected_nodes;
        controller->selectNodes(selected_nodes);
    }

    ImGui::End();

}

}