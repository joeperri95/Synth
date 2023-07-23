#include "TremoloWidget.h"
#include "spdlog/spdlog.h"

namespace ui {

int TremoloWidget::count;

TremoloWidget::TremoloWidget(int id, DisplayContext context) {
    this->amplitude = 0.5;
    this->frequency = 5.0;
    this->_name = "Tremolo-" + std::to_string(++TremoloWidget::count);
    this->_id = id;
    this->nextSubscriberID = 1;
    ImGui::SetCurrentContext(context.imgui_context);
}

TremoloWidget::~TremoloWidget() {
}

void TremoloWidget::render() {
    ImGui::Begin(this->_name.c_str());

    float amp_start = this->amplitude;
    float amp_end = this->amplitude;
    float freq_start = this->frequency;
    float freq_end = this->frequency;

    ImGui::SliderFloat("amplitude", &amp_end, 0.0f, 1.0f);
    this->amplitude = amp_end;

    ImGui::SliderFloat("frequency", &freq_end, 0.0f, 10.0f);
    this->frequency = freq_end;

    if (amp_start != amp_end) {
        spdlog::info("Tremolo amplitude changed to: {}", amp_end);
        this->notify();
    }

    if (freq_start != freq_end) {
        spdlog::info("Tremolo frequency changed to: {}", freq_end);
        this->notify();
    }

    ImGui::End();
}

void TremoloWidget::notify() {
    spdlog::debug("TremoloWidget::notify");
    AudioParameterMap args;
    AudioParameter amplitude;
    AudioParameter frequency;
    frequency.setParamFloat(this->frequency);
    amplitude.setParamFloat(this->amplitude);
    args["frequency"] = frequency;
    args["amplitude"] = amplitude;

    for(const auto& it : this->subscribers) {
        it.second(this->nodeArgs[it.first], args, this->args[it.first]);
    }
}

int TremoloWidget::addSubscriber(int nodeid, AudioParameterCallback func, void * arg) {
    spdlog::debug("TremoloWidget::addSubscriber");
    int ret = nextSubscriberID++;
    this->subscribers[ret] = func;
    this->args[ret] = arg;
    this->nodeArgs[ret] = nodeid;
    return ret;
}

void TremoloWidget::removeSubscriber(int id) {
    spdlog::debug("TremoloWidget::removeSubscriber");
    auto it = this->subscribers.find(id);
    if (it != this->subscribers.end()) {
        this->subscribers.erase(it);
    }
}

extern "C" {
    void build_control(int id, DisplayContext context, ControlWidget ** control) {
        *control = new TremoloWidget(id, context);
    }
}

}
