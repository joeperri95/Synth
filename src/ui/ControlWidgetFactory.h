#pragma once

#include <vector>
#include <memory> 
#include "spdlog/spdlog.h"

#include "ControlWidget.h"

namespace ui {

class ControlWidgetFactory {
public:
    ControlWidgetFactory();
    ~ControlWidgetFactory();
    std::shared_ptr<ControlWidget> create(int id, std::string s); 
};

}