#include "PassthroughNodeWidget.h"

namespace ui {

PassthroughNodeWidget::PassthroughNodeWidget(int id, int input, int output) {
    this->input=input;
    this->output=output;
    this->id=id;
}
PassthroughNodeWidget::~PassthroughNodeWidget() {}

void PassthroughNodeWidget::render() {
    ImNodes::BeginNode(this->id);
    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted("passthrough");
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
