#include "BiquadNode.h"

namespace pipeline {

BiquadNode::BiquadNode(NodeID id, AttrID inputID, AttrID outputID){
    this->_id = id;
    this->numInputs = 1;
    this->numOutputs = 1;
    this->inputID = inputID;
    this->outputID = outputID;
    this->inputs[inputID] = nullptr;
    this->outputs[outputID] = nullptr;
    done = false;

    this->format = audio::AudioFormat(CHANNELS, SAMPLE_RATE, BIT_DEPTH, BUFF_SIZE);

    this->cutoff = 10000.0f;
    this->type = FilterType::lowpass;
    this->updateParameters(this->cutoff, static_cast<uint32_t>(type));

    this->input = std::make_shared<audio::AudioQueue<sample_type>>();
    this->input->setFormat(format);
    this->input->setQueue(this->inputs[inputID]);

    this->output = std::make_shared<audio::AudioQueue<sample_type>>();
    this->output->setFormat(format);
    this->output->setQueue(this->outputs[outputID]);

    this->updateThread = std::thread(BiquadNode::update, this);
}

BiquadNode::~BiquadNode(){
    this->done = true;
    this->updateThread.join();
}

void BiquadNode::update(BiquadNode *self) {
    int period_us = self->format.getPeriodMicroSeconds();

    while(!self->done) {
        auto now = std::chrono::high_resolution_clock::now();
        auto sleep = now + std::chrono::microseconds(period_us);
        for (int i = 0; i < self->format.bufferSize; i++) {
            sample_type xm1 = 0.0f;
            sample_type xm2 = 0.0f;
            sample_type ym1 = 0.0f;
            sample_type ym2 = 0.0f;
            sample_type y = 0.0f;
            if(!self->input->empty()) {
                sample_type x = self->input->pop();

                y = x * self->b0 + xm1 * self->b1 + xm2 * self->b2 - ym1 * self->a1 - ym2 * self->a2;

                ym2 = ym1;
                ym1 = y;
                xm2 = xm1;
                xm1 = x;

                if(!self->output->full()) {
                    self->output->push(y);
                }
            } else {
                //spdlog::debug("BiquadNode::update input queue is empty and isQueueValid == {}", self->input->isQueueValid());
            }
        }
        std::this_thread::sleep_until(sleep);
    }
}

void BiquadNode::onInputChanged(AttrID attr) {
    spdlog::debug("BiquadNode::onInputChanged");
    if(this->outputs.find(attr) != this->outputs.end()) {
        this->output->setQueue(this->outputs[attr]);
    }
    else if(this->inputs.find(attr) != this->inputs.end()) {
        this->input->setQueue(this->inputs[attr]);
    }
    else {
        spdlog::warn("Cannot find attr in inputs or outputs");
    }
}

void BiquadNode::updateParameters(float cutoff, uint32_t filter_type) {
    this->type = FilterType(filter_type);
    this->cutoff = cutoff;

    // Parameters to be used in calculating coefficients
    float w0 = M_PI * 2 * this->cutoff / this->format.sampleRate;
    float cos_w0 = cos(w0);
    float sin_w0 = sin(w0);
    float Q = 0.77; // butterworth
    float alpha = sin_w0 / (2 * Q);
    float a0 = 1 + alpha;

    switch(this->type) {
        case FilterType::lowpass:
            spdlog::debug("BiquadNode: Changed to lowpass filter");
            this->b0 = (1.0 - cos_w0) / (2.0 * a0);
            this->b1 = (1.0 - cos_w0) / (a0);
            this->b2 = (1.0 - cos_w0) / (2.0 * a0);
            this->a1 = (-2.0 * cos_w0) / (a0);
            this->a2 = (1.0 - alpha) / (a0);
            break;
        case FilterType::highpass:
            spdlog::debug("BiquadNode: Changed to highpass filter");
            this->b0 = (1.0 + cos_w0) / (2.0 * a0);
            this->b1 = -1.0 * (1 + cos_w0) / (a0);
            this->b2 = (1.0 + cos_w0) / (2.0 * a0);
            this->a1 = (-2 * cos_w0) / (a0);
            this->a2 = (1 - alpha) / (a0);
            break;
        case FilterType::bandpass:
            spdlog::debug("BiquadNode: Changed to bandpass filter");
            this->b0 = (sin_w0) / (2 * a0);
            this->b1 = 0.0;
            this->b2 = -1.0 * (sin_w0) / (2 * a0);
            this->a1 = (-2 * cos_w0) / (a0);
            this->a2 = (1 - alpha) / (a0);
            break;
        default:
            spdlog::warn("BiquadNode: invalid filter type detected");
            break;
    }
}

void BiquadNode::onStateChanged(std::map<std::string, AudioParameter> newState, void *arg) {
    bool change = false;
    float new_cutoff = this->cutoff;
    uint32_t new_filter_type = static_cast<uint32_t>(this->type);
    auto it = newState.find("cutoff");
    if(it != newState.end()) {
        AudioParameter cutoffParam = newState["cutoff"];
        if(cutoffParam.getType() != AudioParameterType::TYPE_FLOAT) {
            spdlog::warn("BiquadNode::onStateChange did not receive a float parameter for \"cutoff\"");
        } else {
            spdlog::info("BiquadNode::onStateChange changing cutoff parameter");
            change = true;
            new_cutoff = cutoffParam.getParamFloat();
        }
    } else {
        spdlog::warn("BiquadNode::onStateChange did not find cutoff parameter");
    }

    it = newState.find("filter_type");
    if(it != newState.end()) {
        AudioParameter filterTypeParam = newState["filter_type"];
        if(filterTypeParam.getType() != AudioParameterType::TYPE_UINT32) {
            spdlog::warn("BiquadNode::onStateChange did not receive a uint32_t parameter for \"filter_type\"");
        } else {
            spdlog::info("BiquadNode::onStateChange changing filter_type parameter");
            change = true;
            new_filter_type = filterTypeParam.getParamU32();
        }
    } else {
        spdlog::warn("BiquadNode::onStateChange did not find filter_type parameter");
    }

    if (change) {
        updateParameters(new_cutoff, new_filter_type);
    }

}

extern "C" {
    int build_node(int id, int nextAttrId, Node ** node) {
        *node = new BiquadNode(id, nextAttrId, nextAttrId + 1);
        return nextAttrId + 2;
    }
}

} // pipeline
