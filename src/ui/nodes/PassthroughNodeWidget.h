#pragma once

#include <vector>

#include "NodeWidget.h"

namespace ui {
class PassthroughNodeWidget : public NodeWidget {
public:
    PassthroughNodeWidget(int id, int input, int output);
    ~PassthroughNodeWidget();

    void render() override;

protected:
    int input;
    int output;
};

}