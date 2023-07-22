
#include "BiquadWidget.h"
#include "spdlog/spdlog.h"

namespace ui {

int BiquadWidget::count;

BiquadWidget::BiquadWidget(int id, float cutoff, FilterType type) {
    this->cutoff = cutoff;
    this->type = type;
    this->_name = "Biquad-" + std::to_string(++BiquadWidget::count);
    this->_id = id;
    this->nextSubscriberID = 1;
}

BiquadWidget::~BiquadWidget() {
}

void BiquadWidget::render() {
    ImGui::Begin(this->_name.c_str());

    float start = this->cutoff;
    float end = this->cutoff;
    ImGui::SliderFloat("Cutoff frequency", &end, 0.0f, 22000.0f);
    this->cutoff = end;

    if (start != end) {
        spdlog::info("Cutoff frequency changed to: {}", end);
        this->notify();
    }

    int e = static_cast<int>(this->type);
    ImGui::RadioButton("Low pass", &e, 0);
    ImGui::RadioButton("High pass", &e, 1);
    ImGui::RadioButton("Band pass", &e, 2);

    if (static_cast<int>(this->type) != e) {
        spdlog::info("Filter type changed to: {}", static_cast<int>(this->type));
        this->notify();
    }

    this->type = BiquadWidget::FilterType(e);


    ImGui::End();
}

void BiquadWidget::notify() {
    spdlog::debug("BiquadWidget::notify");
    AudioParameterMap args;
    AudioParameter cutoff_param;
    cutoff_param.setParamFloat(this->cutoff);
    args["cutoff"] = cutoff_param;
    AudioParameter filter_type_param;
    filter_type_param.setParamU32(static_cast<uint32_t>(this->type));
    args["filter_type"] = filter_type_param;
    for(const auto& it : this->subscribers) {
        it.second(this->nodeArgs[it.first], args, this->args[it.first]);
    }
}

int BiquadWidget::addSubscriber(int nodeid, AudioParameterCallback func, void * arg) {
    spdlog::debug("BiquadWidget::addSubscriber");
    int ret = nextSubscriberID++;
    this->subscribers[ret] = func;
    this->args[ret] = arg;
    this->nodeArgs[ret] = nodeid;
    return ret;
}

void BiquadWidget::removeSubscriber(int id) {
    spdlog::debug("BiquadWidget::removeSubscriber");
    auto it = this->subscribers.find(id);
    if (it != this->subscribers.end()) {
        this->subscribers.erase(it);
    }
}
}
