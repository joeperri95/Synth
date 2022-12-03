#pragma once

#include <vector>

#include "NodeWidget.h"

namespace ui {
class SinkNodeWidget : public NodeWidget {
public:
    SinkNodeWidget(int id, int input);
    ~SinkNodeWidget();

    void render();

protected:
    int input;
};

}