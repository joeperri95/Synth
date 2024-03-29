#include "VolumeWidget.h"
#include "spdlog/spdlog.h"

namespace ui {

int VolumeWidget::count;

VolumeWidget::VolumeWidget(int id, DisplayContext context) {
    this->volume = 0.5;
    this->_name = "volume-" + std::to_string(++VolumeWidget::count);
    this->_id = id;
    this->nextSubscriberID = 1;
    ImGui::SetCurrentContext(context.imgui_context);
}

VolumeWidget::~VolumeWidget() {
}

void VolumeWidget::render() {
    ImGui::Begin(this->_name.c_str());

    float start = this->volume;
    float end = this->volume;
    ImGui::SliderFloat("volume", &end, 0.0f, 2.0f);
    this->volume = end;

    if (start != end) {
        spdlog::info("Volume changed to: {}", end);
        this->notify();
    }

    ImGui::End();
}

void VolumeWidget::notify() {
    spdlog::debug("VolumeWidget::notify");
    AudioParameterMap args;
    AudioParameter volume;
    volume.setParamFloat(this->volume);
    args["volume"] = volume;
    for(const auto& it : this->subscribers) {
        it.second(this->nodeArgs[it.first], args, this->args[it.first]);
    }
}

int VolumeWidget::addSubscriber(int nodeid, AudioParameterCallback func, void * arg) {
    spdlog::debug("VolumeWidget::addSubscriber");
    int ret = nextSubscriberID++;
    this->subscribers[ret] = func;
    this->args[ret] = arg;
    this->nodeArgs[ret] = nodeid;
    return ret;
}

void VolumeWidget::removeSubscriber(int id) {
    spdlog::debug("VolumeWidget::removeSubscriber");
    auto it = this->subscribers.find(id);
    if (it != this->subscribers.end()) {
        this->subscribers.erase(it);
    }
}

extern "C" {
    void build_control(int id, DisplayContext context, ControlWidget ** control) {
        *control = new VolumeWidget(id, context);
    }
}
}
