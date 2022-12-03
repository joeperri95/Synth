#include "SinkNodeWidget.h"

namespace ui {

SinkNodeWidget::SinkNodeWidget(int id, int input) {
    this->input=input;
    this->id=id;
    this->title = "sink";
}
SinkNodeWidget::~SinkNodeWidget() {}

void SinkNodeWidget::render() {
    ImNodes::BeginNode(this->id);
    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(this->title.c_str());
    ImNodes::EndNodeTitleBar();
    ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
    ImNodes::BeginInputAttribute(this->input);
    ImGui::Text("input pin");
    ImNodes::EndInputAttribute();
    ImNodes::PopAttributeFlag();
    ImNodes::EndNode();
}

}