#include "include/algorithm/node.h"

Node::Node(float min, float max)
{
    this->min = min;
    this->max = max;
    this->isLeaf = false;
}

void Node::operator+=(Node *other)
{
    if (other != NULL)
    {
        this->child.push_back(other);

        // update max and min
        if (other->max > this->max)
            this->max = other->max;
        if (other->min < this->min)
            this->min = other->min;
    }
}

Hit Node::intersect(const Ray &ray)
{
    Hit hit;

    // isovalues of voxel
    int p = gridWidth * gridHeight;
    float iso_000 = gridData[2 * (this->index + p) + 1].z;
    float iso_001 = gridData[2 * (this->index) + 1].z;
    float iso_010 = gridData[2 * (this->index + p + gridWidth) + 1].z;
    float iso_011 = gridData[2 * (this->index + gridWidth) + 1].z;
    float iso_100 = gridData[2 * (this->index + p + 1) + 1].z;
    float iso_101 = gridData[2 * (this->index + 1) + 1].z;
    float iso_110 = gridData[2 * (this->index + p + gridWidth + 1) + 1].z;
    float iso_111 = gridData[2 * (this->index + gridWidth + 1) + 1].z;

    // x0, yo, z0 and x1, y1, z1
    float x0 = gridData[2 * (this->index + p)].x;
    float y0 = gridData[2 * (this->index + p)].y;
    float z0 = gridData[2 * (this->index + p)].z;
    float x1 = gridData[2 * (this->index + gridWidth + 1)].x;
    float y1 = gridData[2 * (this->index + gridWidth + 1)].y;
    float z1 = gridData[2 * (this->index + gridWidth + 1)].z;

    // xa + t * xb
    float xa = ray.start.x;
    float xb = ray.dir.x;

    // ya + t * yb
    float ya = ray.start.y;
    float yb = ray.dir.y;

    // za + t * zb
    float za = ray.start.z;
    float zb = ray.dir.z;

    // p = a + t * b
    float u0a = (x1 - xa) / (x1 - x0);
    float v0a = (y1 - ya) / (y1 - y0);
    float w0a = (z1 - za) / (z1 - z0);

    float u0b = xb / (x1 - x0);
    float v0b = yb / (y1 - y0);
    float w0b = zb / (z1 - z0);

    float u1a = (xa - x0) / (x1 - x0);
    float v1a = (ya - y0) / (y1 - y0);
    float w1a = (za - z0) / (z1 - z0);

    float u1b = -xb / (x1 - x0);
    float v1b = -yb / (y1 - y0);
    float w1b = -zb / (z1 - z0);

    


    return hit;
}