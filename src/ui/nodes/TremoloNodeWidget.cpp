#include "TremoloNodeWidget.h"

namespace ui {

TremoloNodeWidget::TremoloNodeWidget(int id, int input, int output) {
    this->input=input;
    this->output=output;
    this->id=id;
}
TremoloNodeWidget::~TremoloNodeWidget() {}

void TremoloNodeWidget::render() {
    ImNodes::BeginNode(this->id);
    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted("Tremolo");
    ImNodes::EndNodeTitleBar();
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

}
