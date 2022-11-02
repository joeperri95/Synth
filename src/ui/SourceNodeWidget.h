#pragma once

#include <vector>

#include "NodeWidget.h"

namespace ui {
class SourceNodeWidget : public NodeWidget {
public:
    SourceNodeWidget(int id, int output);
    ~SourceNodeWidget();

    void render();

protected:
    int output;
};

}