#include "include/algorithm/node.h"

Node::Node(float min, float max)
{
    this->min = min;
    this->max = max;
    this->isLeaf = false;
}

void Node::operator+=(Node *other)
{
    if (other != 0)
    {
        this->child.push_back(other);

        // update max and min
        if (other->max > this->max)
            this->max = other->max;
        if (other->min < this->min)
            this->min = other->min;
    }
}
