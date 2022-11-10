#include "AttributeInfo.h"


AttributeInfo::AttributeInfo(std::vector<int> inputs, std::vector<int> outputs) : inputs(inputs), outputs(outputs) {
}

AttributeInfo::~AttributeInfo() {
}

int AttributeInfo::getInput(int index) {
    return inputs.at(index);
}

int AttributeInfo::getOutput(int index) {
    return outputs.at(index);
}

int AttributeInfo::getInputsCount() {
    return inputs.size();
}

int AttributeInfo::getOutputsCount() {
    return outputs.size();
}
