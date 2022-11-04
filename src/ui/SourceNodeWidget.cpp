#include "SourceNodeWidget.h"

namespace ui {

SourceNodeWidget::SourceNodeWidget(int id, int output) {
    this->output=output;
    this->id=id;
}
SourceNodeWidget::~SourceNodeWidget() {}

void SourceNodeWidget::render() {
    ImNodes::BeginNode(this->id);
    ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
    ImNodes::BeginOutputAttribute(output);
    ImGui::Text("output pin");
    ImNodes::EndOutputAttribute();
    ImNodes::PopAttributeFlag();
    ImNodes::EndNode();
}

}