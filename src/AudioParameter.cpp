#include "AudioParameter.h"


AudioParameter::AudioParameter() : type(AudioParameterType::TYPE_NOTHING), data{} {}

AudioParameter::AudioParameter(const AudioParameter &other) {
    this->type = other.type;
    switch (this->type) {
        case AudioParameterType::TYPE_NOTHING:
        this->data = {};
        break;
        case AudioParameterType::TYPE_BOOL:
        this->data.boolean_ = other.data.boolean_;
        break;
        case AudioParameterType::TYPE_FLOAT:
        this->data.float_ = other.data.float_;
        break;
        case AudioParameterType::TYPE_DOUBLE:
        this->data.double_ = other.data.double_;
        break;
        case AudioParameterType::TYPE_UINT8:
        this->data.uint8_ = other.data.uint8_;
        break;
        case AudioParameterType::TYPE_UINT16:
        this->data.uint16_ = other.data.uint16_;
        break;
        case AudioParameterType::TYPE_UINT32:
        this->data.uint32_ = other.data.uint32_;
        break;
        case AudioParameterType::TYPE_UINT64:
        this->data.uint64_ = other.data.uint64_;
        break;
        case AudioParameterType::TYPE_INT8:
        this->data.int8_ = other.data.int8_;
        break;
        case AudioParameterType::TYPE_INT16:
        this->data.int16_ = other.data.int16_;
        break;
        case AudioParameterType::TYPE_INT32:
        this->data.int32_ = other.data.int32_;
        break;
        case AudioParameterType::TYPE_INT64:
        this->data.int64_ = other.data.int64_;
        break;
        default:
        break;
    }
}

AudioParameter::~AudioParameter(){}

AudioParameterType AudioParameter::getType(){
    return this->type;
}

void    AudioParameter::setParamNothing() {
    this->type = AudioParameterType::TYPE_NOTHING;
}

float    AudioParameter::getParamFloat(){
    return this->data.float_;
}
void    AudioParameter::setParamFloat(float data){
    this->type = AudioParameterType::TYPE_FLOAT;
    this->data.float_ = data;
}

double   AudioParameter::getParamDouble(){
    return this->data.double_;
}
void   AudioParameter::setParamDouble(double data){
    this->type = AudioParameterType::TYPE_DOUBLE;
    this->data.double_ = data;
}

bool     AudioParameter::getParamBool(){
    return this->data.boolean_;
}
void     AudioParameter::setParamBool(bool data){
    this->type = AudioParameterType::TYPE_BOOL;
    this->data.boolean_ = data;
}

uint8_t  AudioParameter::getParamU8(){
    return this->data.uint8_;
}
void  AudioParameter::setParamU8(uint8_t data){
    this->type = AudioParameterType::TYPE_UINT8;
    this->data.uint8_ = data;
}

uint16_t AudioParameter::getParamU16(){
    return this->data.uint16_;
}
void AudioParameter::setParamU16(uint16_t data){
    this->type = AudioParameterType::TYPE_UINT16;
    this->data.uint16_ = data;
}

uint32_t AudioParameter::getParamU32(){
    return this->data.uint32_;
}
void AudioParameter::setParamU32(uint32_t data){
    this->type = AudioParameterType::TYPE_UINT32;
    this->data.uint32_ = data;
}

uint64_t AudioParameter::getParamU64(){
    return this->data.uint64_;
}
void AudioParameter::setParamU64(uint64_t data){
    this->type = AudioParameterType::TYPE_UINT64;
    this->data.uint64_ = data;
}

int8_t   AudioParameter::getParamI8(){
    return this->data.int8_;
}
void   AudioParameter::setParamI8(int8_t data){
    this->type = AudioParameterType::TYPE_INT8;
    this->data.int8_ = data;
}

int16_t  AudioParameter::getParamI16(){
    return this->data.int16_;
}
void  AudioParameter::setParamI16(int16_t data){
    this->type = AudioParameterType::TYPE_INT16;
    this->data.uint16_ = data;
}

int32_t  AudioParameter::getParamI32(){
    return this->data.int32_;
}

void  AudioParameter::setParamI32(int32_t data){
    this->type = AudioParameterType::TYPE_INT32;
    this->data.int32_ = data;
}

int64_t  AudioParameter::getParamI64(){
    return this->data.int64_;
}
void  AudioParameter::setParamI64(int64_t data){
    this->type = AudioParameterType::TYPE_INT64;
    this->data.int64_ = data;
}
