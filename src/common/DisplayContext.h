#pragma once

#include "imgui.h"
#include "implot.h"
#include "../external/imnodes/imnodes.h"

struct DisplayContext {
    ImGuiContext *imgui_context;
    ImNodesContext *imnodes_context;
    ImPlotContext *implot_context;
};
