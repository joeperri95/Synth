#pragma once

#include <vector>

#include "NodeWidget.h"

namespace ui {
class MixerNodeWidget : public NodeWidget {
public:
    MixerNodeWidget(int id, int input1, int input2, int output);
    ~MixerNodeWidget();

    void render();

protected:
    int input1;
    int input2;
    int output;
};

}