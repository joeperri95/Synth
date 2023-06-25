#pragma once

#include <vector>
#include <memory> 
#include <iostream>
#include "nlohmann/json.hpp"

#include "NodeWidget.h"

namespace ui { namespace nodes {

struct Attribute { 
    std::string type;
    std::string text;
};

struct Recipe {
    std::string title;    
    std::vector<Attribute> attributes;
};

class NodeWidgetFactory {
public:
    NodeWidgetFactory();
    ~NodeWidgetFactory();

    std::shared_ptr<NodeWidget> create(int id, std::string s); 
    void loadRecord(std::string filename);

private:
    int nextAttrID;
    std::map<std::string, Recipe> recipes;
};

}}