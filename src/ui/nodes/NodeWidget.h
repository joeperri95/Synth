#pragma once

#include <vector>
#include <string>
#include "spdlog/spdlog.h"

#include "../../../external/imnodes/imnodes.h"

namespace ui { namespace nodes {
    
struct AttrData {
    std::string type;
    std::string text;
    int id;
};

struct NodeData {
    std::string title;
    std::vector<AttrData> attributes;
};

class NodeWidget {
public:
    NodeWidget(int id, NodeData data);
    ~NodeWidget();

    void render();
    void onSelected();
    void setTitle(std::string title);

protected:
    int id;
    bool selected;
    NodeData data;
};

}}