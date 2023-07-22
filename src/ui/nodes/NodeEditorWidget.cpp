#include "NodeEditorWidget.h"
#include "NodeWidget.h"
#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace ui { namespace nodes {

NodeEditorWidget::NodeEditorWidget(std::shared_ptr<PipelineController> controller) {
    this->controller = controller;
    this->loadRecord("etc/recipes.json");
#define TESTING
#ifdef TESTING
    // Create a default pipeline
    controller->addNode("wav");
    controller->addNode("passthrough");
    controller->addNode("vibrato");
    controller->addNode("volume");
    controller->addNode("biquad");
    controller->addNode("sink");
    controller->addLink(1, 2);
    controller->addLink(3, 4);
    controller->addLink(5, 6);
    controller->addLink(7, 8);
    controller->addLink(9, 10);

#endif
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
        for (const auto &item : this->contextItems) {
            if (ImGui::MenuItem(item.c_str())) {
                controller->addNode(item);
            }
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

    ImNodes::MiniMap();
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

void NodeEditorWidget::loadRecord(std::string filename) {
    std::ifstream ifs(filename);

    json j = json::parse(ifs);
    json recipe_list = j["recipes"];

    for (const auto &i: recipe_list) {
        std::string title(i["title"]);
        this->contextItems.push_back((title));
    }
}

}}
