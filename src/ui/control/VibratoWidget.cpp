#include "VibratoWidget.h"
#include "spdlog/spdlog.h"

namespace ui {

int VibratoWidget::count;

VibratoWidget::VibratoWidget(int id, int depth, float frequency) {
    this->depth = depth;
    this->frequency = frequency;
    this->_name = "vibrato-" + std::to_string(++VibratoWidget::count);
    this->_id = id;
    this->nextSubscriberID = 1;
}

VibratoWidget::~VibratoWidget() {
}

void VibratoWidget::render() {
    ImGui::Begin(this->_name.c_str());

    int amp_start = this->depth;
    int amp_end = this->depth;
    float freq_start = this->frequency;
    float freq_end = this->frequency;

    ImGui::SliderInt("depth", &amp_end, 1, 100);
    this->depth = amp_end;
    
    ImGui::SliderFloat("frequency", &freq_end, 0.0f, 20.0f);
    this->frequency = freq_end;
    
    if (amp_start != amp_end) {
        spdlog::info("Vibrato depth changed to: {}", amp_end);
        this->notify();
    }

    if (freq_start != freq_end) {
        spdlog::info("Vibrato frequency changed to: {}", freq_end);
        this->notify();
    }

    ImGui::End();
}

void VibratoWidget::notify() {
    spdlog::debug("VibratoWidget::notify");
    AudioParameterMap args;
    AudioParameter depth;
    AudioParameter frequency;
    frequency.setParamFloat(this->frequency);
    depth.setParamFloat(this->depth);
    args["frequency"] = frequency;
    args["depth"] = depth;

    for(const auto& it : this->subscribers) {
        it.second(this->nodeArgs[it.first], args, this->args[it.first]);
    }
}

int VibratoWidget::addSubscriber(int nodeid, AudioParameterCallback func, void * arg) {
    spdlog::debug("VibratoWidget::addSubscriber");
    int ret = nextSubscriberID++;
    this->subscribers[ret] = func; 
    this->args[ret] = arg; 
    this->nodeArgs[ret] = nodeid; 
    return ret;
}

void VibratoWidget::removeSubscriber(int id) {
    spdlog::debug("VibratoWidget::removeSubscriber");
    auto it = this->subscribers.find(id);
    if (it != this->subscribers.end()) {
        this->subscribers.erase(it);
    }
}
}