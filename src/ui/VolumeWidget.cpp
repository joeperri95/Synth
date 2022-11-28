#include "VolumeWidget.h"
#include "spdlog/spdlog.h"

namespace ui {

VolumeWidget::VolumeWidget(int id, float volume) {
    this->volume = volume;
    this->_name = "volume";
    this->_id = id;
    this->nextSubscriberID = 1;
}

VolumeWidget::~VolumeWidget() {
}

void VolumeWidget::render() {
    ImGui::Begin(this->_name.c_str());

    float start = this->volume;
    float end = this->volume;
    ImGui::SliderFloat("volume", &end, 0.0f, 1.0f);
    this->volume = end;
    
    if (start != end) {
        spdlog::info("Volume changed to: {}", end);
        this->notify();
    }

    ImGui::End();
}

void VolumeWidget::notify() {
    spdlog::debug("VolumeWidget::notify");
    for(const auto& it : this->subscribers) {
        it.second();
    }
}

int VolumeWidget::addSubscriber(std::function<void()> func) {
    spdlog::debug("VolumeWidget::addSubscriber");
    int ret = nextSubscriberID++;
    this->subscribers[ret] = func; 
    return ret;
}

void VolumeWidget::removeSubscriber(int id) {
    spdlog::debug("VolumeWidget::removeSubscriber");
    auto it = this->subscribers.find(id);
    if (it != this->subscribers.end()) {
        this->subscribers.erase(it);
    }
}
}