#include "ControlWidgetFactory.h"
#include "VolumeWidget.h"
#include "TremoloWidget.h"
#include "VibratoWidget.h"
#include "SineControlWidget.h"
#include "BiquadWidget.h"

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
    } else if (s.compare("vibrato") == 0) {
        std::shared_ptr<ControlWidget> ret = std::make_unique<VibratoWidget>(id, 20, 5.0);
        return ret;
    } else if (s.compare("biquad") == 0) {
        std::shared_ptr<ControlWidget> ret = std::make_unique<BiquadWidget>(id, 440.0, BiquadWidget::FilterType::lowpass);
        return ret;
    } else {
        spdlog::warn("Control recipe not found {}", s);
        return nullptr;
    }
}

}
