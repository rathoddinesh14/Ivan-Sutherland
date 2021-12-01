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
        node->tl = vertices[2 * index1];
        node->tr = vertices[2 * index2];
        node->bl = vertices[2 * index5];
        node->br = vertices[2 * index6];
        node->btl = vertices[2 * index4];
        node->btr = vertices[2 * index3];
        node->bbl = vertices[2 * index8];
        node->bbr = vertices[2 * index7];

        // todo: logic for calculating tl, tr, bl, br

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
    
    node->tl.x = __FLT_MAX__;
    node->tl.y = -__FLT_MAX__;
    node->tl.z = -__FLT_MAX__;

    node->tr.x = -__FLT_MAX__;
    node->tr.y = -__FLT_MAX__;
    node->tr.z = -__FLT_MAX__;
    
    node->bl.x = __FLT_MAX__;
    node->bl.y = __FLT_MAX__;
    node->bl.z = -__FLT_MAX__;
    
    node->br.x = -__FLT_MAX__;
    node->br.y = __FLT_MAX__;
    node->br.z = -__FLT_MAX__;

    node->btl.x = __FLT_MAX__;
    node->btl.y = -__FLT_MAX__;
    node->btl.z = __FLT_MAX__;

    node->btr.x = -__FLT_MAX__;
    node->btr.y = -__FLT_MAX__;
    node->btr.z = __FLT_MAX__;
    
    node->bbl.x = __FLT_MAX__;
    node->bbl.y = __FLT_MAX__;
    node->bbl.z = __FLT_MAX__;
    
    node->bbr.x = -__FLT_MAX__;
    node->bbr.y = __FLT_MAX__;
    node->bbr.z = __FLT_MAX__;

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

vector<Node *> DomainSearch::getNodes(float isovalue, Node *node)
{
    vector<Node *> nodes;

    if (node == NULL)
        return nodes;

    if (node->min <= isovalue && node->max >= isovalue)
    {
        if (node->isLeaf)
            nodes.push_back(node);

        for (unsigned int i = 0; i < node->child.size(); i++)
        {
            if (node->child[i]->min <= isovalue && node->child[i]->max >= isovalue)
            {
                vector<Node *> childNodes = getNodes(isovalue, node->child[i]);
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

static Vector3f ds_p0, ds_p1, ds_p2, ds_p4, ds_intersectP;
double ds_plane[4];
static Hit ds_hit;
static float ds_num, ds_denom;

void DomainSearch::rayIntersection(Ray& ray, float isovalue, Node *node, Hit *hit)
{

    // cout << "rayIntersection" << endl;

    if (node == NULL)
        return;
    
    if (node->min <= isovalue && node->max >= isovalue)
    {

        // cout << "node->min <= isovalue && node->max >= isovalue" << endl;

        // TODO: ray hits leaf node
        if (node->isLeaf)
        {
            ds_hit = node->intersect(ray);
            // ds_hit.position.Print();
            // printf("\n");
            if (ds_hit.t > 0 && (hit->t < 0 || ds_hit.t < hit->t))
            {
                hit->position = ds_hit.position;
                hit->t = ds_hit.t;
                hit->normal = ds_hit.normal;
                hit->material = ds_hit.material;
                // printf("hit position %f %f %f\n", hit->position.x, hit->position.y, hit->position.z);
            }
        }
        else
        {
            // front face
            GetPlane(ds_plane, node->tl, Vector3f(0.0f, 0.0f, 1.0f));
            // TODO: logic for intersection with plane
            ds_num = - (ds_plane[3] * ray.start.x + ds_plane[2] * ray.start.y + ds_plane[1] * ray.start.z + ds_plane[0]);
            ds_denom = ds_plane[3] * ray.dir.x + ds_plane[2] * ray.dir.y + ds_plane[1] * ray.dir.z;
            if (ds_denom != 0.0)
            {
                float t = ds_num / ds_denom;
                if (t >= 0)
                {
                    ds_intersectP = ray.start + ray.dir * t;

                    // check if the intersection point is inside the triangle
                    if (IsInsideTriangle(node->bl, node->br, node->tr, ds_intersectP) || 
                            IsInsideTriangle(node->bl, node->tr, node->tl, ds_intersectP))
                    {
                        // TODO : recursive call
                        for (unsigned int i = 0; i < node->child.size(); i++)
                        {
                            if (node->child[i]->min <= isovalue && node->child[i]->max >= isovalue)
                            {
                                rayIntersection(ray, isovalue, node->child[i], hit);
                            }
                        }
                        return;
                    }
                }
            }

            // top face
            GetPlane(ds_plane, node->tl, Vector3f(0.0f, 1.0f, 0.0f));
            // TODO: logic for intersection with plane
            ds_num = - (ds_plane[3] * ray.start.x + ds_plane[2] * ray.start.y + ds_plane[1] * ray.start.z + ds_plane[0]);
            ds_denom = ds_plane[3] * ray.dir.x + ds_plane[2] * ray.dir.y + ds_plane[1] * ray.dir.z;
            if (ds_denom != 0.0)
            {
                float t = ds_num / ds_denom;
                if (t >= 0)
                {
                    ds_intersectP = ray.start + ray.dir * t;

                    // check if the intersection point is inside the triangle
                    if (IsInsideTriangle(node->tl, node->tr, node->btr, ds_intersectP) || 
                            IsInsideTriangle(node->tl, node->btr, node->btl, ds_intersectP))
                    {
                        // TODO : recursive call
                        for (unsigned int i = 0; i < node->child.size(); i++)
                        {
                            if (node->child[i]->min <= isovalue && node->child[i]->max >= isovalue)
                            {
                                rayIntersection(ray, isovalue, node->child[i], hit);
                            }
                        }
                        return;
                    }
                }
            }

            // left face
            GetPlane(ds_plane, node->tl, Vector3f(-1.0f, 0.0f, 0.0f));
            // TODO: logic for intersection with plane
            ds_num = - (ds_plane[3] * ray.start.x + ds_plane[2] * ray.start.y + ds_plane[1] * ray.start.z + ds_plane[0]);
            ds_denom = ds_plane[3] * ray.dir.x + ds_plane[2] * ray.dir.y + ds_plane[1] * ray.dir.z;
            if (ds_denom != 0.0)
            {
                float t = ds_num / ds_denom;
                if (t >= 0)
                {
                    ds_intersectP = ray.start + ray.dir * t;

                    // check if the intersection point is inside the triangle
                    if (IsInsideTriangle(node->bbl, node->bl, node->tl, ds_intersectP) || 
                            IsInsideTriangle(node->bbl, node->tl, node->btl, ds_intersectP))
                    {
                        // TODO : recursive call
                        for (unsigned int i = 0; i < node->child.size(); i++)
                        {
                            if (node->child[i]->min <= isovalue && node->child[i]->max >= isovalue)
                            {
                                rayIntersection(ray, isovalue, node->child[i], hit);
                            }
                        }
                        return;
                    }
                }
            }

            // right face
            GetPlane(ds_plane, node->tr, Vector3f(1.0f, 0.0f, 0.0f));
            // TODO: logic for intersection with plane
            ds_num = - (ds_plane[3] * ray.start.x + ds_plane[2] * ray.start.y + ds_plane[1] * ray.start.z + ds_plane[0]);
            ds_denom = ds_plane[3] * ray.dir.x + ds_plane[2] * ray.dir.y + ds_plane[1] * ray.dir.z;
            if (ds_denom != 0.0)
            {
                float t = ds_num / ds_denom;
                if (t >= 0)
                {
                    ds_intersectP = ray.start + ray.dir * t;

                    // check if the intersection point is inside the triangle
                    if (IsInsideTriangle(node->br, node->bbr, node->btr, ds_intersectP) || 
                            IsInsideTriangle(node->br, node->btr, node->tr, ds_intersectP))
                    {
                        // TODO : recursive call
                        for (unsigned int i = 0; i < node->child.size(); i++)
                        {
                            if (node->child[i]->min <= isovalue && node->child[i]->max >= isovalue)
                            {
                                rayIntersection(ray, isovalue, node->child[i], hit);
                            }
                        }
                        return;
                    }
                }
            }

            // bottom face
            GetPlane(ds_plane, node->bl, Vector3f(0.0f, -1.0f, 0.0f));
            // TODO: logic for intersection with plane
            ds_num = - (ds_plane[3] * ray.start.x + ds_plane[2] * ray.start.y + ds_plane[1] * ray.start.z + ds_plane[0]);
            ds_denom = ds_plane[3] * ray.dir.x + ds_plane[2] * ray.dir.y + ds_plane[1] * ray.dir.z;
            if (ds_denom != 0.0)
            {
                float t = ds_num / ds_denom;
                if (t >= 0)
                {
                    ds_intersectP = ray.start + ray.dir * t;

                    // check if the intersection point is inside the triangle
                    if (IsInsideTriangle(node->bbl, node->bbr, node->br, ds_intersectP) || 
                            IsInsideTriangle(node->bbl, node->br, node->bl, ds_intersectP))
                    {
                        // TODO : recursive call
                        for (unsigned int i = 0; i < node->child.size(); i++)
                        {
                            if (node->child[i]->min <= isovalue && node->child[i]->max >= isovalue)
                            {
                                rayIntersection(ray, isovalue, node->child[i], hit);
                            }
                        }
                        return;
                    }
                }
            }
        }
    }
}

void DomainSearch::rayIntersection(Ray& ray, float isovalue, Hit *hit)
{
    return rayIntersection(ray, isovalue, root, hit);
}