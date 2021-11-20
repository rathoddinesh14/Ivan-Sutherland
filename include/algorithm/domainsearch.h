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
};

#endif // __DOMAINSEARCH_H__