#pragma once

#include "ControlWidget.h"
#include "common/DisplayContext.h"
#include <map>
#include <functional>

namespace ui {

class VolumeWidget : public ControlWidget {
public:
    VolumeWidget(int id, DisplayContext context);
    ~VolumeWidget();
    void render() override;

    void notify();
    int addSubscriber(int nodeid, AudioParameterCallback func, void * arg);
    void removeSubscriber(int id);

protected:
    float volume;
    int nextSubscriberID;
    std::map<int, AudioParameterCallback> subscribers;
    std::map<int, void *> args;
    std::map<int, int> nodeArgs;
private:
    static int count;
};

extern "C" {
   void build_control(int id, DisplayContext ctx, ControlWidget ** control);
}

}
