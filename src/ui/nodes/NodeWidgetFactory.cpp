#include "NodeWidgetFactory.h"
#include "NodeWidget.h"
#include "ui/nodes/NodeWidget.h"
#include <fstream>

using json = nlohmann::json;
namespace ui { namespace nodes {

NodeWidgetFactory::NodeWidgetFactory() : nextAttrID(1), recipes() {
    loadRecord("etc/recipes.json");
}

NodeWidgetFactory::~NodeWidgetFactory() {}

std::shared_ptr<NodeWidget> NodeWidgetFactory::create(int id, std::string s) {
    auto it = this->recipes.find(s);
    if (it != this->recipes.end()) {
        std::vector<AttrData> attrs;
        for (const auto &j : it->second.attributes) {
            attrs.push_back({.type = j.type, .text = j.text, .id = nextAttrID++});
        }
        NodeData data {.title = it->second.title, .attributes = attrs};
        std::shared_ptr<NodeWidget> ret = std::make_unique<NodeWidget>(id, data);
        return ret;
    } else {
        spdlog::warn("Node recipe not found {}", s);
        return nullptr;
    }
}

void NodeWidgetFactory::loadRecord(std::string filename) {
    std::ifstream ifs(filename);

    json j = json::parse(ifs);    
    json recipe_list = j["recipes"];    
    
    for (const auto &i: recipe_list) {
        std::string title(i["title"]);
        std::vector<Attribute> attrs;
        for (const auto &j: i["attributes"]) {
            Attribute attribute {
               .type = j["type"],
               .text = j["text"]
            };
            attrs.push_back(attribute);
        }
        this->recipes[title] = {.title = title, .attributes = attrs};
    }
}

}}