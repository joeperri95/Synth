#include "AudioParameter.h"

AudioParameter::AudioParameter() : type(AudioParameterType::TYPE_NOTHING), data{} {}

AudioParameter::AudioParameter(const AudioParameter &other) {
    this->type = other.type;
    this->data = other.data;
}

AudioParameter::~AudioParameter(){}

AudioParameter& AudioParameter::operator=(const AudioParameter &other) {
    this->type = other.type;
    this->data = other.data;
    return *this;
}

AudioParameterType AudioParameter::getType(){
    return this->type;
}

void AudioParameter::setParamNothing() {
    this->type = AudioParameterType::TYPE_NOTHING;
}

float AudioParameter::getParamFloat(){
    return std::get<float>(this->data);
}
void AudioParameter::setParamFloat(float data){
    this->type = AudioParameterType::TYPE_FLOAT;
    this->data = data;
}

double AudioParameter::getParamDouble(){
    return std::get<double>(this->data);
}
void AudioParameter::setParamDouble(double data){
    this->type = AudioParameterType::TYPE_DOUBLE;
    this->data = data;
}

bool AudioParameter::getParamBool(){
    return std::get<bool>(this->data);
}
void AudioParameter::setParamBool(bool data){
    this->type = AudioParameterType::TYPE_BOOL;
    this->data = data;
}

uint8_t AudioParameter::getParamU8(){
    return std::get<uint8_t>(this->data);
}
void AudioParameter::setParamU8(uint8_t data){
    this->type = AudioParameterType::TYPE_UINT8;
    this->data = data;
}

uint16_t AudioParameter::getParamU16(){
    return std::get<uint16_t>(this->data);
}
void AudioParameter::setParamU16(uint16_t data){
    this->type = AudioParameterType::TYPE_UINT16;
    this->data = data;
}

uint32_t AudioParameter::getParamU32(){
    return std::get<uint32_t>(this->data);
}
void AudioParameter::setParamU32(uint32_t data){
    this->type = AudioParameterType::TYPE_UINT32;
    this->data = data;
}

uint64_t AudioParameter::getParamU64(){
    return std::get<uint64_t>(this->data);
}
void AudioParameter::setParamU64(uint64_t data){
    this->type = AudioParameterType::TYPE_UINT64;
    this->data = data;
}

int8_t AudioParameter::getParamI8(){
    return std::get<int8_t>(this->data);
}
void AudioParameter::setParamI8(int8_t data){
    this->type = AudioParameterType::TYPE_INT8;
    this->data = data;
}

int16_t AudioParameter::getParamI16(){
    return std::get<int16_t>(this->data);
}
void AudioParameter::setParamI16(int16_t data){
    this->type = AudioParameterType::TYPE_INT16;
    this->data = data;
}

int32_t AudioParameter::getParamI32(){
    return std::get<int32_t>(this->data);
}

void AudioParameter::setParamI32(int32_t data){
    this->type = AudioParameterType::TYPE_INT32;
    this->data = data;
}

int64_t AudioParameter::getParamI64(){
    return std::get<int64_t>(this->data);
}
void AudioParameter::setParamI64(int64_t data){
    this->type = AudioParameterType::TYPE_INT64;
    this->data = data;
}

std::string AudioParameter::getParamString(){
    return std::get<std::string>(this->data);
}
void AudioParameter::setParamString(std::string data){
    this->type = AudioParameterType::TYPE_STR;
    this->data = data;
}
