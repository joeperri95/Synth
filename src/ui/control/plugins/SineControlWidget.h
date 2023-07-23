#pragma once
#include "ControlWidget.h"
#include "common/DisplayContext.h"
#include <map>
#include <functional>

namespace ui {

class SineControlWidget : public ControlWidget {
public:
    SineControlWidget(int id, DisplayContext ctx);
    ~SineControlWidget();
    void render() override;

    void notify() override;
    int addSubscriber(int nodeid, AudioParameterCallback func, void * arg) override;
    void removeSubscriber(int id) override;

protected:
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
