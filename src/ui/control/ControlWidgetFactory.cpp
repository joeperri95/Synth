#include "ControlWidgetFactory.h"
#include "VolumeWidget.h"
#include "TremoloWidget.h"
#include "SineControlWidget.h"

namespace ui {

ControlWidgetFactory::ControlWidgetFactory() {}

ControlWidgetFactory::~ControlWidgetFactory() {}

std::shared_ptr<ControlWidget> ControlWidgetFactory::create(int id, std::string s) {
    if (s.compare("volume") == 0) {
        std::shared_ptr<ControlWidget> ret = std::make_unique<VolumeWidget>(id, 0.5);
        return ret;
    } else if (s.compare("sine") == 0) {
        std::shared_ptr<ControlWidget> ret = std::make_unique<SineControlWidget>(id, 220);
        return ret;
    } else if (s.compare("tremolo") == 0) {
        std::shared_ptr<ControlWidget> ret = std::make_unique<TremoloWidget>(id, 0.5, 5.0);
        return ret;
    } else {
        spdlog::warn("Recipe not found");
        return nullptr;
    }
}

}