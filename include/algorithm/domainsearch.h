#ifndef __DOMAINSEARCH_H__
#define __DOMAINSEARCH_H__

#include <iostream>
#include <vector>
#include <utility>
#include "include/common/math_utils.h"
#include "node.h"

using namespace std;

class DomainSearch
{
public:
    Node *root;
    Vector3f *vertices;

    DomainSearch(int width, int height, int depth, float *data, Vector3f *vertices);
    ~DomainSearch();

    /**
     * input : start index and end index
     */
    Node *buildOctree(int x, int y, int z, int ex, int ey, int ez, float *data, int w, int h);

    /**
     * input : isovalue
     * output : list of nodes which have isovalue
     */
    vector<Node *> getNodes(float isovalue, Node *root);

    vector<Node *> getNodes(float isovalue);

    /**
     * @brief ray intersection with the sub domain
     * @input: ray, iso value, Node
     * @output: void
     */
    void rayIntersection(Ray& ray, float isovalue, Node *node, Hit *hit);
    void rayIntersection(Ray& ray, float isovalue, Hit *hit);
};

#endif // __DOMAINSEARCH_H__