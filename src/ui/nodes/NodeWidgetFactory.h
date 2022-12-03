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
    std::shared_ptr<NodeWidget> create(int id, std::string s); 

private:
    int nextAttrID;
};

}