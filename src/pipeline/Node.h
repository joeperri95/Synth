#pragma once

#include <vector>
#include <cstdint>

namespace pipeline {

typedef uint64_t LinkID;
typedef uint64_t NodeID;

class Node {
public:
    Node();
    virtual ~Node() = 0;
    virtual void update() = 0;
protected:
    NodeID _id;

};

}