#include "NodeWidgetFactory.h"
#include "VolumeNodeWidget.h"
#include "SourceNodeWidget.h"
#include "SinkNodeWidget.h"

namespace ui {

NodeWidgetFactory::NodeWidgetFactory() : nextAttrID(1) {}

NodeWidgetFactory::~NodeWidgetFactory() {}

std::unique_ptr<NodeWidget> NodeWidgetFactory::create(int id, std::string s) {
    if (s.compare("volume") == 0) {
        std::unique_ptr<NodeWidget> ret = std::make_unique<VolumeNodeWidget>(id, nextAttrID, nextAttrID + 1);
        nextAttrID += 2;
        return ret;
    } else if (s.compare("source") == 0) {
        std::unique_ptr<NodeWidget> ret = std::make_unique<SourceNodeWidget>(id, nextAttrID);
        nextAttrID += 1;
        return ret;
    } else if (s.compare("sink") == 0) {
        std::unique_ptr<NodeWidget> ret = std::make_unique<SinkNodeWidget>(id, nextAttrID);
        nextAttrID += 1;
        return ret;
    }
    else {
        std::cout << "Recipe not found" << std::endl;
        return nullptr;
    }
}

}