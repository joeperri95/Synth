#include "BasicPluginControl.h"
#include "spdlog/spdlog.h"

namespace ui {

int BasicPluginControl::count;

BasicPluginControl::BasicPluginControl(int id) {
    this->_name = "basic-" + std::to_string(++BasicPluginControl::count);
    this->_id = id;
    this->nextSubscriberID = 1;
}

BasicPluginControl::~BasicPluginControl() {
}

void BasicPluginControl::render() {
    ImGui::Begin(this->_name.c_str());
    ImGui::End();
}

void BasicPluginControl::notify() {
    spdlog::debug("BasicPluginControl::notify");
    AudioParameterMap args;
    AudioParameter volume;
    for(const auto& it : this->subscribers) {
        it.second(this->nodeArgs[it.first], args, this->args[it.first]);
    }
}

int BasicPluginControl::addSubscriber(int nodeid, AudioParameterCallback func, void * arg) {
    spdlog::debug("BasicPluginControl::addSubscriber");
    int ret = nextSubscriberID++;
    this->subscribers[ret] = func; 
    this->args[ret] = arg; 
    this->nodeArgs[ret] = nodeid; 
    return ret;
}

void BasicPluginControl::removeSubscriber(int id) {
    spdlog::debug("BasicPluginControl::removeSubscriber");
    auto it = this->subscribers.find(id);
    if (it != this->subscribers.end()) {
        this->subscribers.erase(it);
    }
}
}