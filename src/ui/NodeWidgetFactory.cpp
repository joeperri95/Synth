#include "NodeWidgetFactory.h"
#include "VolumeNodeWidget.h"

namespace ui {

NodeWidgetFactory::NodeWidgetFactory() : nextAttrID(1) {}

NodeWidgetFactory::~NodeWidgetFactory() {}

std::unique_ptr<NodeWidget> NodeWidgetFactory::create(std::string s) {
    if (s.compare("Volume") == 0) {
        std::make_unique<VolumeNodeWidget>(0, nextAttrID, nextAttrID + 1);
        nextAttrID += 2;
    }
    else {
        std::cout << "Recipe not found" << std::endl;
    }
}

}