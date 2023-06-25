/*
    Alter NodeWidget.h to make it a concrete class
    Add constructor with params
    The factory will just create a node based on params
    Remove all of the other Node subclasses
    
    {
        title: "blaNode",
        attributes: [
            {
                type: input,
                text: "input 1" 
            }
        ]

    }

*/

#include "NodeWidget.h"
#include <spdlog/spdlog.h>

namespace ui { namespace nodes {

NodeWidget::NodeWidget(int id, NodeData data) : id(id), selected(false), data(data) {}
NodeWidget::~NodeWidget() {}

void NodeWidget::render() {
    ImNodes::BeginNode(this->id);
    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(this->data.title.c_str());
    ImNodes::EndNodeTitleBar();
    ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);

    for (const auto &it: this->data.attributes) {
        if(it.type == "input") {
            ImNodes::BeginInputAttribute(it.id);
            ImGui::Text("%s", it.text.c_str());
            ImNodes::EndInputAttribute();
        } else if (it.type == "output"){
            ImNodes::BeginOutputAttribute(it.id);
            ImGui::Text("%s", it.text.c_str());
            ImNodes::EndOutputAttribute();
        } else {
           spdlog::warn("Attribute type is not input or output. Ignoring"); 
        }
    }
    ImNodes::PopAttributeFlag();
    ImNodes::EndNode();
}

void NodeWidget::onSelected() {
    spdlog::info("Node {} got clicked", id);
}

void NodeWidget::setTitle(std::string title) {
    this->data.title = title;
}

}}
