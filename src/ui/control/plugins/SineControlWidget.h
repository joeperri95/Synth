#pragma once
#include "ControlWidget.h"
#include <map>
#include <functional>

namespace ui {

class SineControlWidget : public ControlWidget {
public:
    SineControlWidget(int id);
    ~SineControlWidget();
    void render() override;

    void notify();
    int addSubscriber(int nodeid, AudioParameterCallback func, void * arg);
    void removeSubscriber(int id); 

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
   void build_control(int id, ControlWidget ** control);
}

}