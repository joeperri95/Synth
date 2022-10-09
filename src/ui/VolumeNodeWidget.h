#pragma once

#include <vector>

#include "NodeWidget.h"

namespace ui {
class VolumeNodeWidget : public NodeWidget {
public:
    VolumeNodeWidget();
    ~VolumeNodeWidget();

    void render();

protected:
    int input;
    int output;
};

}