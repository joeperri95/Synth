#pragma once

#include "ControlWidget.h"
#include <map>
#include <functional>

namespace ui {

class BasicPluginControl : public ControlWidget {
public:
    BasicPluginControl(int id);
    ~BasicPluginControl();
    void render() override;

    void notify();
    int addSubscriber(int nodeid, AudioParameterCallback func, void * arg);
    void removeSubscriber(int id); 

protected:
    int nextSubscriberID;
    std::map<int, AudioParameterCallback> subscribers;
    std::map<int, void *> args;
    std::map<int, int> nodeArgs;
private:
    static int count;
};

}