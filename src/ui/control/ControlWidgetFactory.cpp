#include "ControlWidgetFactory.h"
#include "ui/control/ControlWidget.h"
#include <memory>
#include <dlfcn.h>
#include "nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

namespace ui {

ControlWidgetFactory::ControlWidgetFactory(DisplayContext context) {
    this->context = context;
    loadRecord("etc/recipes.json");
}

ControlWidgetFactory::~ControlWidgetFactory() {}

std::shared_ptr<ControlWidget> ControlWidgetFactory::create(int id, std::string s) {
    if(recipes.contains(s)) {
        if(recipes[s].size() > 0){
            std::shared_ptr<ControlWidget> ret = load(id, s);
            return ret;
        } else {
            spdlog::debug("Recipe {} has no control library", s);
            return nullptr;
        }
    } else {
        spdlog::warn("Control recipe not found {}", s);
        return nullptr;
    }
}

std::shared_ptr<ControlWidget> ControlWidgetFactory::load(int id, std::string recipe) {
    std::string filename(recipes[recipe]);

    void * handle = dlopen(filename.c_str(), RTLD_LAZY);
    if (handle == nullptr) {
        spdlog::error("Issue opening library {} {}", filename, dlerror());
        exit(1);
    }

    void * function_return = dlsym(handle, "build_control");

    if (function_return == nullptr) {
        spdlog::error("Issue loading symbol build_control {}", dlerror());
        exit(1);
    }

    void (*function)(int, DisplayContext, ControlWidget **) = reinterpret_cast<void (*)(int, DisplayContext, ControlWidget **)>(function_return);

    if (function == nullptr) {
        spdlog::error("Issue forming function from symbol build_node {}", dlerror());
        exit(1);
    }

    ControlWidget *control = nullptr;
    function(id, this->context, &control);

    if(control == nullptr) {
        spdlog::error("Issue creating control widget in library file");
        exit(1);
    }

    std::shared_ptr<ControlWidget> ret = std::unique_ptr<ControlWidget>(control);
    return ret;
}

void ControlWidgetFactory::loadRecord(std::string filename) {
    std::ifstream ifs(filename);

    json j = json::parse(ifs);
    json recipe_list = j["recipes"];

    for (const auto &i: recipe_list) {
        std::string title(i["title"]);
        std::string library(i["control_library"]);
        recipes[title] = library;
    }
}

}
