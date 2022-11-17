#pragma once

#include <vector>
#include <string>

typedef enum AttributeType {
    INPUT,
    OUTPUT,
};

struct Attribute {
    int id;
    std::string name;
    bool required;
    AttributeType type;
};

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