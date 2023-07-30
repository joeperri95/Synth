#include "FileSourceNode.h"

namespace pipeline {

FileSourceNode::FileSourceNode(NodeID id, AttrID outputID){
    this->_id = id;
    this->numInputs = 0;
    this->numOutputs = 1;
    this->outputID = outputID;
    this->outputs[outputID] = nullptr;
    done = false;

    this->filename = std::string("");
    this->format = audio::AudioFormat(CHANNELS, SAMPLE_RATE, BIT_DEPTH, BUFF_SIZE);

    this->output = std::make_shared<audio::AudioQueue<sample_type>>();
    this->output->setFormat(format);
    this->output->setQueue(this->outputs[outputID]);

    this->index = 0;
    this->updating = true;

    this->updateThread = std::thread(FileSourceNode::update, this);
}

FileSourceNode::~FileSourceNode(){
    this->done = true;
    this->updateThread.join();
}

void FileSourceNode::update(FileSourceNode *self) {
    int period_us = self->format.getPeriodMicroSeconds();

    while(!self->done) {
        auto now = std::chrono::high_resolution_clock::now();
        auto sleep = now + std::chrono::microseconds(period_us);

        if (self->output->isQueueValid() && !self->updating) {
            for(int i = 0; i < self->format.bufferSize; i++) {
                for(int j = 0; j < self->format.channels; j++) {
                    if (!self->output->full()) {
                        self->output->push(self->data[self->index++]);
                        if(self->index > self->data.size()) {
                            self->index = 0;
                        }
                    }
                }
            }
        } else {
            for(int i = 0; i < self->format.bufferSize; i++) {
                for(int j = 0; j < self->format.channels; j++) {
                    if (!self->output->full()) {
                        self->output->push(0);
                    }
                }
            }
        }

        std::this_thread::sleep_until(sleep);
    }
}

void FileSourceNode::onInputChanged(AttrID attr) {
    if(this->outputs.find(attr) != this->outputs.end()) {
        this->output->setQueue(this->outputs[attr]);
    }
}

void FileSourceNode::onStateChanged(std::map<std::string, AudioParameter> newState, void */*arg*/) {
    auto it = newState.find("filename");
    if(it != newState.end()) {
        AudioParameter param = newState["filename"];
        if(param.getType() != AudioParameterType::TYPE_STR) {
            spdlog::warn("FileSourceNode::onStateChange did not receive a string parameter");
        } else {
            spdlog::info("FileSourceNode::onStateChange changing file to {}", param.getParamString());
            this->filename = param.getParamString();
            readWavData();
        }
    } else {
        spdlog::warn("FileSourceNode::onStateChange did not find volume parameter");
    }
}

void FileSourceNode::readWavData() {
    std::ifstream fs;

    spdlog::info("Starting to read {}", filename);
    spdlog::info("current path {}", std::filesystem::current_path().string());

    fs.open(this->filename);
    if (!fs.is_open()) {
        spdlog::error("Could not open wav file");
        return;
    }

    fs.ignore(4); // RIFF
    fs.ignore(4); // file size
    fs.ignore(4); // WAVE
    fs.ignore(4); // fmt
    fs.ignore(4); // fmt_length
    fs.ignore(2); // fmt type
    fs.ignore(2); // channels
    fs.ignore(4); // sample_rate
    fs.ignore(4); // byte rate
    fs.ignore(2); // block align
    fs.ignore(2); // bits_per_sample

    char _data[5];
    char _fileSize[5];

    fs.get(_data, 5);
    fs.get(_fileSize, 5);

    //look for 'data' section there may be other sections we don't care about
    while (std::string("data").compare(_data) != 0)
    {
        fs.ignore(*(int *)_fileSize);
        fs.get(_data, 5);
        fs.get(_fileSize, 5);
    }

    int length = *(int *)_fileSize + 1;

    updating = true;
    this->data.resize(length);

    char temp[2];

    //only handle the case the data is a 16 bit int for now
    //TODO implement other data types

    for (int i = 0; i < (length >> 1); i++)
    {
        fs.read(temp, 2);
        this->data[i] = *(short *)temp;
    }

    updating = false;

    spdlog::info("Done reading wav file");
    fs.close();
}


extern "C" {
    int build_node(int id, int nextAttrId, Node ** node) {
        *node = new FileSourceNode(id, nextAttrId);
        return nextAttrId + 1;
    }
}

} // pipeline
