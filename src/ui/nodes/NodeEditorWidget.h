#pragma once

#include <vector>
#include <map>

#include "../../../external/imnodes/imnodes.h"
#include "SDL_keycode.h"
#include "NodeWidget.h"
#include "common/PipelineController.h"

namespace ui { namespace nodes {
class NodeEditorWidget {
public:
    NodeEditorWidget(std::shared_ptr<PipelineController> controller);
    ~NodeEditorWidget();

    void render();

protected:
    std::string _name;
    int _id;
    // PipelineController controller
    // contains nodes and links
    // communicates to audio pipeline
    std::shared_ptr<PipelineController> controller;

    // Load a list of available nodes to be added to the context menu
    void loadRecord(std::string filename);
    std::vector<std::string> contextItems;

};

}}
