#include "NodeEditorWidget.h"
#include "VolumeNodeWidget.h"
#include "SourceNodeWidget.h"
#include "SinkNodeWidget.h"
#include <iostream>

namespace ui {

NodeEditorWidget::NodeEditorWidget() {
    nextID = 1;
    nextNodeID = 1;
    nextLinkID = 1;
    this->nodes.insert({nextID, std::unique_ptr<NodeWidget>(new SourceNodeWidget(nextID++, nextNodeID++))});
    this->nodes.insert({nextID,std::unique_ptr<NodeWidget>(new SinkNodeWidget(nextID++, nextNodeID++))});
    this->nodes.insert({nextID,std::unique_ptr<NodeWidget>(new VolumeNodeWidget(nextID++, nextNodeID, nextNodeID + 1))});
    nextNodeID += 2;
    this->nodes.insert({nextID,std::unique_ptr<NodeWidget>(new VolumeNodeWidget(nextID++, nextNodeID, nextNodeID + 1))});
    nextNodeID += 2;
}


NodeEditorWidget::~NodeEditorWidget() {}

void NodeEditorWidget::render() {

    ImGui::Begin(this->_name.c_str());

    ImNodes::BeginNodeEditor();

    //for(const auto &it : controller->getNodes()) {
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
            // controller->addNode("volume");
            this->nodes.insert({nextID,std::unique_ptr<NodeWidget>(new VolumeNodeWidget(nextID++, nextNodeID, nextNodeID + 1))});
            nextNodeID += 2;
        }

        if (ImGui::MenuItem("sine"))
        {
            std::cout << "sine" << std::endl;
        }

        if (ImGui::MenuItem("time"))
        {
            std::cout << "time"  << std::endl;
        }

        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();

    // render links
    /*
    auto links = controller->getLinks();
    for (auto it = links.begin(); it != links.end(); it++)
    {
        const std::pair<int, int> p = links[it->first];
        ImNodes::Link(it->first, p.first, p.second);
    }
    */

    for (auto it = links.begin(); it != links.end(); it++)
    {
        const std::pair<int, int> p = links[it->first];
        ImNodes::Link(it->first, p.first, p.second);
    } 

    ImNodes::EndNodeEditor();

    // check for connections
    int start_attr, end_attr;
    if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
    {
        //controller->addLink(start_attr, end_attr);
        links[nextLinkID++] = std::make_pair(start_attr, end_attr);
    }

    // check for removed connections
    int out_attr;
    if (ImNodes::IsLinkDestroyed(&out_attr))
    {
        //controller->removeLink(out_attr);
        links.erase(out_attr);
    }

    // get selected nodes
    int num_selected_nodes = ImNodes::NumSelectedNodes();
    if (num_selected_nodes > 0)
    {
        std::vector<int> selected_nodes;
        selected_nodes.resize(num_selected_nodes);
        ImNodes::GetSelectedNodes(selected_nodes.data());

        for (auto it : selected_nodes) {
            // controller->selectNode(it);
        }
    }

    ImGui::End();

}

}