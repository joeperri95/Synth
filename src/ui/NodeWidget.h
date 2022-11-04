#pragma once

#include <vector>

#include "../../external/imnodes/imnodes.h"

namespace ui {
class NodeWidget {
public:
    NodeWidget();
    virtual ~NodeWidget() = 0;

    virtual void render() = 0;
    virtual void onSelected() {};

protected:
    int id;
    bool selected;
};

inline NodeWidget::NodeWidget() {}
inline NodeWidget::~NodeWidget() {}

}