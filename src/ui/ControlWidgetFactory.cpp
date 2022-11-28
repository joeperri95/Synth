#include "ControlWidgetFactory.h"
#include "VolumeWidget.h"

namespace ui {

ControlWidgetFactory::ControlWidgetFactory() {}

ControlWidgetFactory::~ControlWidgetFactory() {}

std::shared_ptr<ControlWidget> ControlWidgetFactory::create(int id, std::string s) {
    if (s.compare("volume") == 0) {
        std::shared_ptr<ControlWidget> ret = std::make_unique<VolumeWidget>(id, 0.5);
        return ret;
    } else {
        spdlog::warn("Recipe not found");
        return nullptr;
    }
}

}