#pragma once

#include <vector>
#include <memory>
#include "spdlog/spdlog.h"

#include "common/DisplayContext.h"
#include "ControlWidget.h"

namespace ui {

class ControlWidgetFactory {
public:
    ControlWidgetFactory(DisplayContext context);
    ~ControlWidgetFactory();
    std::shared_ptr<ControlWidget> create(int id, std::string s);
    std::shared_ptr<ControlWidget> load(int id, std::string recipe);

private:
    void loadRecord(std::string filename);
    std::map<std::string,std::string> recipes;
    DisplayContext context;
};

}
