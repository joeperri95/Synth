#pragma once

#include <memory>
#include <vector>
#include <string>
#include <functional>

#include "imgui.h"

namespace ui {

class ControlWidget {
public:
    ControlWidget();
    virtual ~ControlWidget() = 0;

    virtual void render() = 0;
    virtual void notify() = 0;
    virtual int addSubscriber(std::function<void()> func) = 0;
    virtual void removeSubscriber(int id) = 0; 


protected:
    std::string _name;
    int _id;
};

inline ControlWidget::ControlWidget() {}
inline ControlWidget::~ControlWidget() {}

} // namespace ui