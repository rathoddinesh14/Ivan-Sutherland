#ifndef __NODE_H__
#define __NODE_H__

#include<vector>

class Node
{
public:
    /* data */
    float min, max;

    // store address of child nodes in vector
    std::vector<Node *> child;
    bool isLeaf;
    int index;

    Node(float min, float max);

    // overload with + operator and add it to the children vector
    void operator+=(Node *other);
};
#endif // __NODE_H__