#pragma once

#include "Node.h"
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <filesystem>
#include "AudioFormat.h"
#include "AudioQueue.h"

namespace pipeline {

class FileSourceNode : public Node {
public:
    FileSourceNode(NodeID node, AttrID inputID);
    ~FileSourceNode();
    void onInputChanged(AttrID attr);
    void onStateChanged(std::map<std::string, AudioParameter> newState, void *args);
    static void update(FileSourceNode *self);
     
private:
    audio::AudioFormat format; 
    std::string filename;
    std::shared_ptr<audio::AudioQueue<sample_type>> output;
    AttrID outputID;

    bool done;
    std::thread updateThread;
    unsigned int index;
    std::vector<sample_type> data;
    void readWavData();
};

}
