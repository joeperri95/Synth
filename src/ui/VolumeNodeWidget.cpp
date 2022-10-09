#include "VolumeNodeWidget.h"

namespace ui {

VolumeNodeWidget::VolumeNodeWidget() {
    input=3;
    output=4;
}
VolumeNodeWidget::~VolumeNodeWidget() {}

void VolumeNodeWidget::render() {
    ImNodes::BeginNode(this->id);
    ImNodes::BeginInputAttribute(input);
    ImGui::Text("input pin");
    ImNodes::EndInputAttribute();
    ImNodes::BeginOutputAttribute(output);
    ImGui::Text("output pin");
    ImNodes::EndOutputAttribute();
    ImNodes::EndNode();

}

}