#include "SourceNodeWidget.h"

namespace ui {

SourceNodeWidget::SourceNodeWidget(int id, int output) {
    this->output=output;
    this->id=id;
    this->title = "source";
}
SourceNodeWidget::~SourceNodeWidget() {}

void SourceNodeWidget::render() {
    ImNodes::BeginNode(this->id);
    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(this->title.c_str());
    ImNodes::EndNodeTitleBar();
    ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
    ImNodes::BeginOutputAttribute(output);
    ImGui::Text("output pin");
    ImNodes::EndOutputAttribute();
    ImNodes::PopAttributeFlag();
    ImNodes::EndNode();
}

}