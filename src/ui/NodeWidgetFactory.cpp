#include "NodeWidgetFactory.h"
#include "VolumeNodeWidget.h"

namespace ui {

NodeWidgetFactory::NodeWidgetFactory() : nextAttrID(1) {}

NodeWidgetFactory::~NodeWidgetFactory() {}

std::unique_ptr<NodeWidget> NodeWidgetFactory::create(int id, std::string s) {
    if (s.compare("volume") == 0) {
        std::unique_ptr<NodeWidget> ret = std::make_unique<VolumeNodeWidget>(id, nextAttrID, nextAttrID + 1);
        nextAttrID += 2;
        return ret;
    }
    else {
        std::cout << "Recipe not found" << std::endl;
        return nullptr;
    }
}

}