#include "include/algorithm/domainsearch.h"

DomainSearch::DomainSearch(int width, int height, int depth, float* data, Vector3f* vertices)
{
    this->root = NULL;
    cout << "===========================" << endl;
    cout << "Building octree..." << endl;
    this->vertices = vertices;
    root = buildOctree(0, 0, 0, width - 1, height - 1, depth - 1, data, width, height);
    cout << "Octree built." << endl;
}

DomainSearch::~DomainSearch()
{
    delete root;
}

Node* DomainSearch::buildOctree(int x, int y, int z, int ex, int ey, int ez, float *data, int w, int h)
{
    if (x == ex || y == ey || z == ez)
    {
        return NULL;
    }

    if (ex - x == 1 && ey - y == 1 && ez - z == 1)
    {
        // base case

        // create 8 index
        int index1 = z * w * h + y * w + x;
        int index2 = z * w * h + y * w + x + 1;
        int index3 = z * w * h + (y + 1) * w + x + 1;
        int index4 = z * w * h + (y + 1) * w + x;
        int index5 = (z + 1) * w * h + y * w + x;
        int index6 = (z + 1) * w * h + y * w + x + 1;
        int index7 = (z + 1) * w * h + (y + 1) * w + x + 1;
        int index8 = (z + 1) * w * h + (y + 1) * w + x;

        pair<float, float> minMax = getMinMax(data[index1], data[index2], data[index3], data[index4],
                                              data[index5], data[index6], data[index7], data[index8]);
        Node *node = new Node(minMax.first, minMax.second);
        node->index = index1;
        node->gridWidth = w;
        node->gridHeight = h;
        node->gridData = vertices;
        node->isLeaf = true;
        return node;
    }

    // // print x,y,z
    // cout << "recursive call at "
    //      << "(" << x << "," << y << "," << z << ") at level " << level <<"\n";

    int mx = x + (ex - x) / 2;
    int my = y + (ey - y) / 2;
    int mz = z + (ez - z) / 2;

    // create a node with float min and max
    Node *node = new Node(__FLT_MAX__, -__FLT_MAX__);
    *node += buildOctree(x, y, z, mx, my, mz, data, w, h);
    *node += buildOctree(mx, y, z, ex, my, mz, data, w, h);
    *node += buildOctree(x, my, z, mx, ey, mz, data, w, h);
    *node += buildOctree(mx, my, z, ex, ey, mz, data, w, h);
    *node += buildOctree(x, y, mz, mx, my, ez, data, w, h);
    *node += buildOctree(mx, y, mz, ex, my, ez, data, w, h);
    *node += buildOctree(x, my, mz, mx, ey, ez, data, w, h);
    *node += buildOctree(mx, my, mz, ex, ey, ez, data, w, h);

    return node;
}

vector<Node *> DomainSearch::getNodes(float isovalue, Node *root)
{
    vector<Node *> nodes;

    if (root == NULL)
        return nodes;

    if (root->min <= isovalue && root->max >= isovalue)
    {
        if (root->isLeaf)
            nodes.push_back(root);

        for (unsigned int i = 0; i < root->child.size(); i++)
        {
            if (root->child[i]->min <= isovalue && root->child[i]->max >= isovalue)
            {
                vector<Node *> childNodes = getNodes(isovalue, root->child[i]);
                nodes.insert(nodes.end(), childNodes.begin(), childNodes.end());
            }
        }
    }

    return nodes;
}

vector<Node *> DomainSearch::getNodes(float isovalue)
{
    return getNodes(isovalue, root);
}