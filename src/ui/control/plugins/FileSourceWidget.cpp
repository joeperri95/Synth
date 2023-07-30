#include "FileSourceWidget.h"
#include "spdlog/spdlog.h"

namespace ui {

int FileSourceWidget::count;

FileSourceWidget::FileSourceWidget(int id, DisplayContext context) : filename(""), nextSubscriberID(1), subscribers(),
                                                                                        args(), nodeArgs(), browser() {
    this->_name = "FileSource-" + std::to_string(++FileSourceWidget::count);
    this->_id = id;
    ImGui::SetCurrentContext(context.imgui_context);
    browser.SetTitle("Select File");
    browser.SetTypeFilters({".wav"});
}

FileSourceWidget::~FileSourceWidget() {
}

void FileSourceWidget::render() {
    ImGui::Begin(this->_name.c_str());

    std::string start = this->filename;
    if(ImGui::Button("Select file")) {
        spdlog::info("Button was pressed");
        browser.Open();
    }

    ImGui::End();
    browser.Display();

    if(browser.HasSelected()) {
        this->filename = browser.GetSelected().string();
        browser.ClearSelected();
    }

    if (start != this->filename) {
        spdlog::info("FileSource changed to: {}", this->filename);
        this->notify();
    }
}

void FileSourceWidget::notify() {
    spdlog::debug("FileSourceWidget::notify");
    AudioParameterMap args;
    AudioParameter FileSource;
    FileSource.setParamString(this->filename);
    args["filename"] = FileSource;
    for(const auto& it : this->subscribers) {
        it.second(this->nodeArgs[it.first], args, this->args[it.first]);
    }
}

int FileSourceWidget::addSubscriber(int nodeid, AudioParameterCallback func, void * arg) {
    spdlog::debug("FileSourceWidget::addSubscriber");
    int ret = nextSubscriberID++;
    this->subscribers[ret] = func;
    this->args[ret] = arg;
    this->nodeArgs[ret] = nodeid;
    return ret;
}

void FileSourceWidget::removeSubscriber(int id) {
    spdlog::debug("FileSourceWidget::removeSubscriber");
    auto it = this->subscribers.find(id);
    if (it != this->subscribers.end()) {
        this->subscribers.erase(it);
    }
}

extern "C" {
    void build_control(int id, DisplayContext context, ControlWidget ** control) {
        *control = new FileSourceWidget(id, context);
    }
}
}
