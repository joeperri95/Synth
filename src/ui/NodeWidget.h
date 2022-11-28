#pragma once

#include <vector>
#include <string>
#include "spdlog/spdlog.h"

#include "../../external/imnodes/imnodes.h"

namespace ui {
class NodeWidget {
public:
    NodeWidget();
    virtual ~NodeWidget() = 0;

    virtual void render() = 0;
    virtual void onSelected();
    void setTitle(std::string title);

protected:
    int id;
    bool selected;
    std::string title;
};

inline NodeWidget::NodeWidget() {}
inline NodeWidget::~NodeWidget() {}
inline void NodeWidget::onSelected() {spdlog::info("Node {} got clicked", id);}
inline void NodeWidget::setTitle(std::string title) {this->title = title;}

}