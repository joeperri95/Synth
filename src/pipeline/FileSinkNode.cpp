
#include "FileSinkNode.h"
#include <iostream>
#include <fstream>
#include <iomanip>

namespace pipeline {

void FileSinkNode::onStateChanged(std::map<std::string, AudioParameter> /*newState*/, void */*args*/) {}

FileSinkNode::FileSinkNode(NodeID node, AttrID inputID) {
    this->_id = node;
    this->numInputs = 1;
    this->numOutputs = 0;
    this->inputs[inputID] = nullptr;
    this->inputID = inputID;
    this->format = audio::AudioFormat(CHANNELS, SAMPLE_RATE, BIT_DEPTH, BUFF_SIZE);

    this->done = false;
    this->validQueue = false;

    this->queue = std::make_shared<audio::AudioQueue<sample_type>>();
    this->queue->setFormat(format);
    this->queue->setQueue(this->inputs[inputID]);

    this->updateThread = std::thread(FileSinkNode::update, this); 
}

FileSinkNode::~FileSinkNode() {}

void FileSinkNode::update(FileSinkNode *self) {

    const std::string fname("output");
    std::ofstream ofs;
    ofs.open(fname);

    if (self->queue == nullptr) {
        return;
    }  

    sample_type sample;
    audio::AudioFormat format = self->queue->getFormat();
    int period_us = static_cast<int>(1000000.0f * (self->format.bufferSize) / (self->format.channels * self->format.sampleRate));

    while (!self->done) {
        auto now = std::chrono::high_resolution_clock::now();
        auto sleep = now + std::chrono::microseconds(period_us);
        for (int i = 0; i < format.bufferSize; i++)
        {
            for (int j = 0; j < format.channels; j++)
            {
                if (self->validQueue) {
                    sample = self->queue->pop();
                    ofs << std::setw(4) << std::hex << std::setfill('0') << sample << std::endl;
                } else {
                    spdlog::debug("FileSinkNode::play_callback queue is invalid");
                }
            }
        }
        std::this_thread::sleep_until(sleep);
    }

    ofs.close();
}

void FileSinkNode::onInputChanged(AttrID attr) {
    spdlog::debug("FileSinkNode::onInputChanged");
    this->queue->setQueue(this->inputs[attr]);
    this->validQueue = this->inputs[attr] != nullptr;
}
     
}