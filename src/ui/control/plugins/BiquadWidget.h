#pragma once

#include "ControlWidget.h"
#include <map>
#include <functional>
#include "common/DisplayContext.h"

namespace ui {

class BiquadWidget : public ControlWidget {
public:
    enum class FilterType {
        lowpass,
        highpass,
        bandpass
    };

    BiquadWidget(int id, DisplayContext context);
    ~BiquadWidget();
    void render() override;

    void notify() override;
    int addSubscriber(int nodeid, AudioParameterCallback func, void * arg) override;
    void removeSubscriber(int id) override;

protected:
    float cutoff;
    FilterType type;
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
