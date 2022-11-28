#pragma once

#include <vector>
#include <map>

#include "ControlWidget.h"
#include "../../external/imnodes/imnodes.h"
#include "SDL_keycode.h"
#include "NodeWidget.h"
#include "common/PipelineController.h"

namespace ui {
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

};
}
