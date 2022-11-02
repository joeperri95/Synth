#pragma once

#include <vector>
#include <map>

#include "ControlWidget.h"
#include "../../external/imnodes/imnodes.h"
#include "SDL_keycode.h"
#include "NodeWidget.h"

namespace ui {
class NodeEditorWidget : ControlWidget {
public:
    NodeEditorWidget();
    ~NodeEditorWidget();
    void render();

protected:
    std::map<int, std::pair<int, int>> links;
    std::map<int, std::unique_ptr<NodeWidget>> nodes;
    int nextID;
    int nextNodeID;
    int nextLinkID;
};
}