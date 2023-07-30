#pragma once

#include "ControlWidget.h"
#include "common/DisplayContext.h"
#include <map>
#include <functional>
#include "../external/imgui-filebrowser/imfilebrowser.h"

namespace ui {

class FileSourceWidget : public ControlWidget {
public:
    FileSourceWidget(int id, DisplayContext context);
    ~FileSourceWidget();
    void render() override;

    void notify() override;
    int addSubscriber(int nodeid, AudioParameterCallback func, void * arg) override;
    void removeSubscriber(int id) override;

protected:
    std::string filename;
    int nextSubscriberID;
    std::map<int, AudioParameterCallback> subscribers;
    std::map<int, void *> args;
    std::map<int, int> nodeArgs;
    ImGui::FileBrowser browser;
private:
    static int count;
};

extern "C" {
   void build_control(int id, DisplayContext ctx, ControlWidget ** control);
}

}
