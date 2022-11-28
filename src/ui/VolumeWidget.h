#pragma once

#include "ControlWidget.h"
#include <map>
#include <functional>

namespace ui {

class VolumeWidget : public ControlWidget {
public:
    VolumeWidget(int id, float volume);
    ~VolumeWidget();
    void render() override;

    void notify();
    int addSubscriber(std::function<void()> func);
    void removeSubscriber(int id); 

protected:
    float volume;
    int nextSubscriberID;
    std::map<int, std::function<void()>> subscribers;

};

}