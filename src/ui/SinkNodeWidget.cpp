#include "SinkNodeWidget.h"

namespace ui {

SinkNodeWidget::SinkNodeWidget(int id, int input) {
    this->input=input;
    this->id=id;
}
SinkNodeWidget::~SinkNodeWidget() {}

void SinkNodeWidget::render() {
    ImNodes::BeginNode(this->id);
    ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
    ImNodes::BeginInputAttribute(this->input);
    ImGui::Text("input pin");
    ImNodes::EndInputAttribute();
    ImNodes::PopAttributeFlag();
    ImNodes::EndNode();
}

}