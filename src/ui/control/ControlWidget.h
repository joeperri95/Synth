#pragma once

#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <map>


#include "imgui.h"
#include "common/AudioParameter.h"

using AudioParameterMap = std::map<std::string, AudioParameter>;
using AudioParameterCallback = std::function<void(int, AudioParameterMap, void*)>;

namespace ui {

class ControlWidget {
public:
    ControlWidget();
    virtual ~ControlWidget() = 0;

    virtual void render() = 0;
    virtual void notify() = 0;
    virtual int addSubscriber(int nodeid, AudioParameterCallback func, void *arg) = 0;
    virtual void removeSubscriber(int id) = 0; 


protected:
    std::string _name;
    int _id;
};

inline ControlWidget::ControlWidget() {}
inline ControlWidget::~ControlWidget() {}

} // namespace ui