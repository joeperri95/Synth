#pragma once

#include <vector>
#include <memory> 
#include <iostream>

#include "NodeWidget.h"

namespace ui {

class NodeWidgetFactory {
public:
    NodeWidgetFactory();
    ~NodeWidgetFactory();
    std::unique_ptr<NodeWidget> create(std::string s); 

private:
    int nextAttrID;
};

}