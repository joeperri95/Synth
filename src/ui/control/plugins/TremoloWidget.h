#pragma once

#include "ControlWidget.h"
#include <map>
#include <functional>
#include "common/DisplayContext.h"

namespace ui {

class TremoloWidget : public ControlWidget {
public:
    TremoloWidget(int id, DisplayContext context);
    ~TremoloWidget();
    void render() override;

    void notify();
    int addSubscriber(int nodeid, AudioParameterCallback func, void * arg);
    void removeSubscriber(int id);

protected:
    float amplitude;
    float frequency;
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
