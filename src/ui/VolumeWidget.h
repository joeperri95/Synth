#pragma once

#include "ControlWidget.h"

namespace ui {

class VolumeWidget : ControlWidget {
public:
    VolumeWidget(std::shared_ptr<float> state);
    ~VolumeWidget();
    void render();

protected:
    std::shared_ptr<float> _state;

};

}