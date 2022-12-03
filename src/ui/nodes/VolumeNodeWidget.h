#pragma once

#include <vector>

#include "NodeWidget.h"

namespace ui {
class VolumeNodeWidget : public NodeWidget {
public:
    VolumeNodeWidget(int id, int input, int output);
    ~VolumeNodeWidget();

    void render() override;

protected:
    int input;
    int output;
};

}