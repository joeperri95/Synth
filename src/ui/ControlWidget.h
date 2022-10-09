#pragma once

#include <memory>
#include <string>

#include "imgui.h"

namespace ui {

class ControlWidget {
public:
    ControlWidget();
    virtual ~ControlWidget() = 0;
    virtual void render() = 0;
protected:
    std::string _name;
};

inline ControlWidget::ControlWidget() {}
inline ControlWidget::~ControlWidget() {}

} // namespace ui