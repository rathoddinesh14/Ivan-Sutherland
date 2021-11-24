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
    int p = gridWidth * gridHeight;
    float iso_000 = gridData[2 * (this->index + p + gridWidth + 1) + 1].z;
    float iso_001 = gridData[2 * (this->index + gridWidth + 1) + 1].z;
    float iso_010 = gridData[2 * (this->index + p + 1) + 1].z;
    float iso_011 = gridData[2 * (this->index + 1) + 1].z;
    float iso_100 = gridData[2 * (this->index + p + gridWidth) + 1].z;
    float iso_101 = gridData[2 * (this->index + gridWidth) + 1].z;
    float iso_110 = gridData[2 * (this->index + p) + 1].z;
    float iso_111 = gridData[2 * (this->index) + 1].z;

    printf("\niso_000: %f\n", iso_000);
    printf("iso_001: %f\n", iso_001);
    printf("iso_010: %f\n", iso_010);
    printf("iso_011: %f\n", iso_011);
    printf("iso_100: %f\n", iso_100);
    printf("iso_101: %f\n", iso_101);
    printf("iso_110: %f\n", iso_110);
    printf("iso_111: %f\n", iso_111);

    // x0, yo, z0 and x1, y1, z1
    float x0 = gridData[2 * (this->index + p + gridWidth + 1)].x;
    float y0 = gridData[2 * (this->index + p + gridWidth + 1)].y;
    float z0 = gridData[2 * (this->index + p + gridWidth + 1)].z;
    float x1 = gridData[2 * (this->index)].x;
    float y1 = gridData[2 * (this->index)].y;
    float z1 = gridData[2 * (this->index)].z;

    // printf("index %d\n", this->index);

    printf("x0: %f, y0: %f, z0: %f\n", x0, y0, z0);
    printf("x1: %f, y1: %f, z1: %f\n", x1, y1, z1);

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

    float u1a = 1 - u0a;
    float v1a = 1 - v0a;
    float w1a = 1 - w0a;

    float u1b = 1 - u0b;
    float v1b = 1 - v0b;
    float w1b = 1 - w0b;

    // At^3 + Bt^2 + Ct + D = 0
    double *C = new double[4];

    C[3] = u0b * v0b * w0b * iso_000 
         + u0b * v0b * w1b * iso_001 
         + u0b * v1b * w0b * iso_010
         + u0b * v1b * w1b * iso_011 
         + u1b * v0b * w0b * iso_100 
         + u1b * v0b * w1b * iso_101 
         + u1b * v1b * w0b * iso_110 
         + u1b * v1b * w1b * iso_111;

    C[2] = (u0a * v0b * w0b + u0b * v0a * w0b + u0b * v0b * w0a) * iso_000 +
           (u0a * v0b * w1b + u0b * v0a * w1b + u0b * v0b * w1a) * iso_001 +
           (u0a * v1b * w0b + u0b * v1a * w0b + u0b * v1b * w0a) * iso_010 +
           (u0a * v1b * w1b + u0b * v1a * w1b + u0b * v1b * w1a) * iso_011 +
           (u1a * v0b * w0b + u1b * v0a * w0b + u1b * v0b * w0a) * iso_100 +
           (u1a * v0b * w1b + u1b * v0a * w1b + u1b * v0b * w1a) * iso_101 +
           (u1a * v1b * w0b + u1b * v1a * w0b + u1b * v1b * w0a) * iso_110 +
           (u1a * v1b * w1b + u1b * v1a * w1b + u1b * v1b * w1a) * iso_111;

    C[1] = (u0b * v0a * w0a + u0a * v0b * w0a + u0a * v0a * w0b) * iso_000 +
           (u0b * v0a * w1a + u0a * v0b * w1a + u0a * v0a * w1b) * iso_001 +
           (u0b * v1a * w0a + u0a * v1b * w0a + u0a * v1a * w0b) * iso_010 +
           (u0b * v1a * w1a + u0a * v1b * w1a + u0a * v1a * w1b) * iso_011 +
           (u1b * v0a * w0a + u1a * v0b * w0a + u1a * v0a * w0b) * iso_100 +
           (u1b * v0a * w1a + u1a * v0b * w1a + u1a * v0a * w1b) * iso_101 +
           (u1b * v1a * w0a + u1a * v1b * w0a + u1a * v1a * w0b) * iso_110 +
           (u1b * v1a * w1a + u1a * v1b * w1a + u1a * v1a * w1b) * iso_111;

    C[0] = u0a * v0a * w0a * iso_000 
         + u0a * v0a * w1a * iso_001 
         + u0a * v1a * w0a * iso_010 
         + u0a * v1a * w1a * iso_011 
         + u1a * v0a * w0a * iso_100 
         + u1a * v0a * w1a * iso_101 
         + u1a * v1a * w0a * iso_110 
         + u1a * v1a * w1a * iso_111 
         - isoValue;

    // C[3] = u0b * v0b * w0b * (iso_000 + iso_001 + iso_010 + iso_011)
    //      + u1b * v1b * w1b * (iso_100 + iso_101 + iso_110 + iso_111);

    // C[2] = (u0a * v0b * w0b + u0b * v0a * w0b + u0b * v0b * w0a) * (iso_000 + iso_001 + iso_010 + iso_011)
    //      + (u1a * v1b * w1b + u1b * v1a * w1b + u1b * v1b * w1a) * (iso_100 + iso_101 + iso_110 + iso_111);

    // C[1] = (u0b * v0a * w0a + u0a * v0b * w0a + u0a * v0a * w0b) * (iso_000 + iso_001 + iso_010 + iso_011)
    //      + (u1b * v1a * w1a + u1a * v1b * w1a + u1a * v1a * w1b) * (iso_100 + iso_101 + iso_110 + iso_111);

    // C[0] = u0a * v0a * w0a * (iso_000 + iso_001 + iso_010 + iso_011)
    //      + u1a * v1a * w1a * (iso_100 + iso_101 + iso_110 + iso_111) - isoValue;

    double *S = new double[3];

    int num = SolveCubic(C, S);

    printf("isovalue %f\n", isoValue);
    printf("num: %d\n", num);

    double closestIntersection = -1;

    for (int i = 0; i < num; i++)
    {

        printf("S[%d]: %f\n", i, S[i]);

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

        printf("hit.position: %f %f %f\n", hit.position.x, hit.position.y, hit.position.z);

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