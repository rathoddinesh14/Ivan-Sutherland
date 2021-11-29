#ifndef __NODE_H__
#define __NODE_H__

#include<vector>
#include "include/raytracing/rayutils.h"
#include "include/raytracing/Roots3And4.h"

class Node : public Intersectable
{
public:
    /* data */
    Vector3f* gridData;
    int gridWidth;
    int gridHeight;
    float min, max;

    // front face of the voxel
    Vector3f tl, tr, bl, br;

    // store address of child nodes in vector
    std::vector<Node *> child;
    bool isLeaf;
    int index;
    float isoValue;

    Node(float min, float max);

    // overload with + operator and add it to the children vector
    void operator+=(Node *other);

    Hit intersect(const Ray& ray);
};
#endif // __NODE_H__