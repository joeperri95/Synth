#include "VolumeNodeWidget.h"

#include <iostream>

namespace ui {

VolumeNodeWidget::VolumeNodeWidget(int id, int input, int output) {
    this->input=input;
    this->output=output;
    this->id=id;
}
VolumeNodeWidget::~VolumeNodeWidget() {}

void VolumeNodeWidget::render() {
    ImNodes::BeginNode(this->id);
    ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
    ImNodes::BeginInputAttribute(input);
    ImGui::Text("input pin");
    ImNodes::EndInputAttribute();
    ImNodes::BeginOutputAttribute(output);
    ImGui::Text("output pin");
    ImNodes::EndOutputAttribute();
    ImNodes::PopAttributeFlag();
    ImNodes::EndNode();
}

void VolumeNodeWidget::onSelected() {
    std::cout << "(" << this->id << ") got clicked" << std::endl;
}

}
