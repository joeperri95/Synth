#pragma once

#include <vector>

class AttributeInfo {

public:
    AttributeInfo(std::vector<int> inputs, std::vector<int> outputs);
    ~AttributeInfo();

    int getInput(int index);
    int getOutput(int index);
    int getInputsCount();
    int getOutputsCount();

private:
    std::vector<int> inputs;
    std::vector<int> outputs;
};