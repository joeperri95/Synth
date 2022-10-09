#include "VolumeWidget.h"

namespace ui {

VolumeWidget::VolumeWidget(std::shared_ptr<float> state) {
    this->_state = state;
    this->_name = "volume";
}

VolumeWidget::~VolumeWidget() {

}

void VolumeWidget::render() {
    ImGui::Begin(this->_name.c_str());

    float f = *this->_state;
    ImGui::SliderFloat("volume", &f, 0.0f, 1.0f);
    *this->_state = f;

    ImGui::End();
}

}