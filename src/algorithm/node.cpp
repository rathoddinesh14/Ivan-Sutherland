#include "include/algorithm/node.h"

Node::Node(float min, float max)
{
    this->min = min;
    this->max = max;
    this->isLeaf = false;
    this->material = new RoughMaterial(Vector3f(0.3, 0, 0.2), Vector3f(2.0, 2.0, 2.0), 20);
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
    // printf("Node::intersect()\n");

    Hit hit;

    // isovalues of voxel
    static int p = gridWidth * gridHeight;
    static float iso_000 = gridData[2 * (this->index + p) + 1].z;
    static float iso_001 = gridData[2 * (this->index) + 1].z;
    static float iso_010 = gridData[2 * (this->index + p + gridWidth) + 1].z;
    static float iso_011 = gridData[2 * (this->index + gridWidth) + 1].z;
    static float iso_100 = gridData[2 * (this->index + p + 1) + 1].z;
    static float iso_101 = gridData[2 * (this->index + 1) + 1].z;
    static float iso_110 = gridData[2 * (this->index + p + gridWidth + 1) + 1].z;
    static float iso_111 = gridData[2 * (this->index + gridWidth + 1) + 1].z;

    // x0, yo, z0 and x1, y1, z1
    static float x0 = gridData[2 * (this->index + p)].x;
    static float y0 = gridData[2 * (this->index + p)].y;
    static float z0 = gridData[2 * (this->index + p)].z;
    static float x1 = gridData[2 * (this->index + gridWidth + 1)].x;
    static float y1 = gridData[2 * (this->index + gridWidth + 1)].y;
    static float z1 = gridData[2 * (this->index + gridWidth + 1)].z;

    // xa + t * xb
    static float xa = ray.start.x;
    static float xb = ray.dir.x;

    // ya + t * yb
    static float ya = ray.start.y;
    static float yb = ray.dir.y;

    // za + t * zb
    static float za = ray.start.z;
    static float zb = ray.dir.z;

    // p = a + t * b
    static float u0a = (x1 - xa) / (x1 - x0);
    static float v0a = (y1 - ya) / (y1 - y0);
    static float w0a = (z1 - za) / (z1 - z0);

    static float u0b = xb / (x1 - x0);
    static float v0b = yb / (y1 - y0);
    static float w0b = zb / (z1 - z0);

    static float u1a = (xa - x0) / (x1 - x0);
    static float v1a = (ya - y0) / (y1 - y0);
    static float w1a = (za - z0) / (z1 - z0);

    static float u1b = -xb / (x1 - x0);
    static float v1b = -yb / (y1 - y0);
    static float w1b = -zb / (z1 - z0);

    // At^3 + Bt^2 + Ct + D = 0
    double *C = new double[4];

    C[0] = u0b * v0b * w0b * iso_000 + u0b * v0b * w1b * iso_001 + u0b * v1b * w0b * iso_010 +
           u0b * v1b * w1b * iso_011 + u1b * v0b * w0b * iso_100 + u1b * v0b * w1b * iso_101 +
           u1b * v1b * w0b * iso_110 + u1b * v1b * w1b * iso_111;

    C[1] = (u0a * v0b * w0b + u0b * v0a * w0b + u0b * v0b * w0a) * iso_000 +
           (u0a * v0b * w1b + u0b * v0a * w1b + u0b * v0b * w1a) * iso_001 +
           (u0a * v1b * w0b + u0b * v1a * w0b + u0b * v1b * w0a) * iso_010 +
           (u0a * v1b * w1b + u0b * v1a * w1b + u0b * v1b * w1a) * iso_011 +
           (u1a * v0b * w0b + u1b * v0a * w0b + u1b * v0b * w0a) * iso_100 +
           (u1a * v0b * w1b + u1b * v0a * w1b + u1b * v0b * w1a) * iso_101 +
           (u1a * v1b * w0b + u1b * v1a * w0b + u1b * v1b * w0a) * iso_110 +
           (u1a * v1b * w1b + u1b * v1a * w1b + u1b * v1b * w1a) * iso_111;

    C[2] = (u0b * v0a * w0a + u0a * v0b * w0a + u0a * v0a * w0b) * iso_000 +
           (u0b * v0a * w1a + u0a * v0b * w1a + u0a * v0a * w1b) * iso_001 +
           (u0b * v1a * w0a + u0a * v1b * w0a + u0a * v1a * w0b) * iso_010 +
           (u0b * v1a * w1a + u0a * v1b * w1a + u0a * v1a * w1b) * iso_011 +
           (u1b * v0a * w0a + u1a * v0b * w0a + u1a * v0a * w0b) * iso_100 +
           (u1b * v0a * w1a + u1a * v0b * w1a + u1a * v0a * w1b) * iso_101 +
           (u1b * v1a * w0a + u1a * v1b * w0a + u1a * v1a * w0b) * iso_110 +
           (u1b * v1a * w1a + u1a * v1b * w1a + u1a * v1a * w1b) * iso_111;

    C[3] = u0a * v0a * w0a * iso_000 + u0a * v0a * w1a * iso_001 + u0a * v1a * w0a * iso_010 +
           u0a * v1a * w1a * iso_011 + u1a * v0a * w0a * iso_100 + u1a * v0a * w1a * iso_101 +
           u1a * v1a * w0a * iso_110 + u1a * v1a * w1a * iso_111 - isoValue;

    double *S = new double[3];

    int num = SolveCubic(C, S);

    double closestIntersection = -1;

    for (int i = 0; i < num; i++)
    {
        if (closestIntersection < 0 && S[i] > 0)
        {
            closestIntersection = S[i];
        }

        if (S[i] > 0 && S[i] < closestIntersection)
        {
            closestIntersection = S[i];
        }
    }

    if (closestIntersection < 0)
    {
        // No intersection
    }
    else
    {
        hit.t = closestIntersection;
        hit.position = ray.start + ray.dir * hit.t;
        hit.material = material;
        /*
    	 * Calculate the normal later
    	 */
        //hit.normal = (hit.position - center) / radius;
    }

    delete[] C;
    delete[] S;

    // printf("%f\n", closestIntersection);
    return hit;
}