#pragma once

#include <vector>

#include "NodeWidget.h"

namespace ui {
class TremoloNodeWidget : public NodeWidget {
public:
    TremoloNodeWidget(int id, int input, int output);
    ~TremoloNodeWidget();

    void render() override;

protected:
    int input;
    int output;
};

}