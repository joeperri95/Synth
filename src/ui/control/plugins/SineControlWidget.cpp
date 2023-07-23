
#include "SineControlWidget.h"
#include "spdlog/spdlog.h"

namespace ui {

int SineControlWidget::count = 1;

const float DEFAULT_FREQUENCY = 220.0f;

SineControlWidget::SineControlWidget(int id, DisplayContext context) {
    this->frequency = DEFAULT_FREQUENCY;
    this->_name = "sine-controller-" + std::to_string(SineControlWidget::count++);
    this->_id = id;
    this->nextSubscriberID = 1;
    ImGui::SetCurrentContext(context.imgui_context);
}

SineControlWidget::~SineControlWidget() {
}

void SineControlWidget::render() {
    ImGui::Begin(this->_name.c_str());

    float start = this->frequency;
    float end = this->frequency;
    ImGui::SliderFloat("frequency", &end, 20.0f, 2000.0f);
    this->frequency = end;

    if (start != end) {
        spdlog::info("frequency changed to: {}", end);
        this->notify();
    }

    ImGui::End();
}

void SineControlWidget::notify() {
    spdlog::debug("SineControlWidget::notify");
    AudioParameterMap args;
    AudioParameter frequency;
    frequency.setParamFloat(this->frequency);
    args["frequency"] = frequency;
    for(const auto& it : this->subscribers) {
        it.second(this->nodeArgs[it.first], args, this->args[it.first]);
    }
}

int SineControlWidget::addSubscriber(int nodeid, AudioParameterCallback func, void * arg) {
    spdlog::debug("SineControlWidget::addSubscriber");
    int ret = nextSubscriberID++;
    this->subscribers[ret] = func;
    this->args[ret] = arg;
    this->nodeArgs[ret] = nodeid;
    return ret;
}

void SineControlWidget::removeSubscriber(int id) {
    spdlog::debug("SineControlWidget::removeSubscriber");
    auto it = this->subscribers.find(id);
    if (it != this->subscribers.end()) {
        this->subscribers.erase(it);
    }
}

extern "C" {
    void build_control(int id, DisplayContext context, ControlWidget ** control) {
        *control = new SineControlWidget(id, context);
    }
}

}
