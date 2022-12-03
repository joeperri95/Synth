#include "MixerNodeWidget.h"

namespace ui {
MixerNodeWidget::MixerNodeWidget(int id, int input1, int input2, int output) {
    this->input1 = input1;
    this->input2 = input2;
    this->output = output;
    this->id = id;
}

MixerNodeWidget::~MixerNodeWidget() {}

void MixerNodeWidget::render() {
    ImNodes::BeginNode(this->id);
    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted("mixer");
    ImNodes::EndNodeTitleBar();
    ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
    ImNodes::BeginInputAttribute(input1);
    ImGui::Text("input 1");
    ImNodes::EndInputAttribute();
    ImNodes::BeginInputAttribute(input2);
    ImGui::Text("input 2");
    ImNodes::EndInputAttribute();
    ImNodes::BeginOutputAttribute(output);
    ImGui::Text("output");
    ImNodes::EndOutputAttribute();
    ImNodes::PopAttributeFlag();
    ImNodes::EndNode();
}

} // ui