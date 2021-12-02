#include "include/algorithm/node.h"

Node::Node(float min, float max)
{
    this->min = min;
    this->max = max;
    this->isLeaf = false;
    this->material = new RoughMaterial(Vector3f(0.3, 0.9, 0.2), Vector3f(2.0, 2.0, 2.0), 20);
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

        // TODO: logic for tl, tr, bl, br
        if (other->tl.x <= this->tl.x && other->tl.y >= this->tl.y && other->tl.z >= this->tl.z)
            this->tl = other->tl;
        if (other->tr.x >= this->tr.x && other->tr.y >= this->tr.y && other->tr.z >= this->tr.z)
            this->tr = other->tr;
        if (other->bl.x <= this->bl.x && other->bl.y <= this->bl.y && other->bl.z >= this->bl.z)
            this->bl = other->bl;
        if (other->br.x >= this->br.x && other->br.y <= this->br.y && other->br.z >= this->br.z)
            this->br = other->br;
        if (other->btl.x <= this->btl.x && other->btl.y >= this->btl.y && other->btl.z <= this->btl.z)
            this->btl = other->btl;
        if (other->btr.x >= this->btr.x && other->btr.y >= this->btr.y && other->btr.z <= this->btr.z)
            this->btr = other->btr;
        if (other->bbl.x <= this->bbl.x && other->bbl.y <= this->bbl.y && other->bbl.z <= this->bbl.z)
            this->bbl = other->bbl;
        if (other->bbr.x >= this->bbr.x && other->bbr.y <= this->bbr.y && other->bbr.z <= this->bbr.z)
            this->bbr = other->bbr;
    }
}

static float Iso_0 = 0, Iso_1 = 0, Iso_2 = 0, Iso_3 = 0;
static float Iso_4 = 0, Iso_5 = 0, Iso_6 = 0, Iso_7 = 0;
static float A1 = 0, B1 = 0, C1 = 0, D1 = 0;
static float E1 = 0, F1 = 0, G1 = 0, H1 = 0;

static Vector3f p0, p1, p2, p4, intersectP;
double plane[4];
double M[4], S[3];
static int numSols = 0;
static double closestIntersection = -1;

Hit Node::intersect(const Ray &ray)
{
    // printf("---------------------------------------\n");
    Hit hit;

    // isovalues of voxel
    int p = gridWidth * gridHeight;
    Iso_0 = gridData[2 * (this->index + p) + 1].z;
    Iso_1 = gridData[2 * (this->index + p + 1) + 1].z;
    Iso_2 = gridData[2 * (this->index) + 1].z;
    Iso_3 = gridData[2 * (this->index + p + gridWidth) + 1].z;
    Iso_4 = gridData[2 * (this->index + 1) + 1].z;
    Iso_5 = gridData[2 * (this->index + gridWidth) + 1].z;
    Iso_6 = gridData[2 * (this->index + p + gridWidth + 1) + 1].z;
    Iso_7 = gridData[2 * (this->index + gridWidth + 1) + 1].z;

    // Vector p0, p7
    p0 = gridData[2 * (this->index + p)];
    p1 = gridData[2 * (this->index + p + 1)];
    p2 = gridData[2 * (this->index)];
    p4 = gridData[2 * (this->index + 1)];
    
    // iterate for 5 face
    for (int i = 0; i < 5; i++)
    {
        GetPlane(plane, p0, Vector3f(0.0f, 0.0f, 1.0f));

        // finding intersection
        float t = - (plane[3] * ray.start.x + plane[2] * ray.start.y + plane[1] * ray.start.z + plane[0]) /
                        (plane[3] * ray.dir.x + plane[2] * ray.dir.y + plane[1] * ray.dir.z);

        // t > 0 intersection point
        if (t > 0)
        {
            // intersection point
            intersectP = ray.start + ray.dir * t;

            // check if inside the voxel
            if (IsInsideTriangle(p0, p1, p4, intersectP) || IsInsideTriangle(p0, p4, p2, intersectP))
            {
                //  A1 = v1+v2+v3+v7-v6-v4-v5-v0
                A1 = Iso_1 + Iso_2 + Iso_3 + Iso_7 - Iso_6 - Iso_4 - Iso_5 - Iso_0;
                //  B1 = v4-v2-v1+v0
                B1 = Iso_4 - Iso_2 - Iso_1 + Iso_0;
                //  C1 = v6-v3-v1+v0
                C1 = Iso_6 - Iso_3 - Iso_1 + Iso_0;
                //  D1 = v5-v3-v2+v0
                D1 = Iso_5 - Iso_3 - Iso_2 + Iso_0;
                //  E1 = v1-v0
                E1 = Iso_1 - Iso_0;
                //  F1 = v2-v0
                F1 = Iso_2 - Iso_0;
                //  G1 = v3-v0
                G1 = Iso_3 - Iso_0;
                //  H1 = v0
                H1 = Iso_0;

                M[3] = A1 * ray.dir.x * ray.dir.y * ray.dir.z;
                M[2] = A1 * (ray.dir.x * ray.dir.y * intersectP.z + ray.dir.x * ray.dir.z * intersectP.y + ray.dir.y * ray.dir.z * intersectP.x)
                        + B1 * ray.dir.x * ray.dir.y + C1 * ray.dir.x * ray.dir.z + D1 * ray.dir.y * ray.dir.z;
                M[1] = ray.dir.x * (A1 * intersectP.y * intersectP.z + B1 * intersectP.y + D1 * intersectP.z + E1)
                    + ray.dir.y * (A1 * intersectP.x * intersectP.z + B1 * intersectP.x + C1 * intersectP.z + F1)
                    + ray.dir.z * (A1 * intersectP.x * intersectP.y + C1 * intersectP.y + D1 * intersectP.x + G1);
                M[0] = - isoValue + GetIsoValue(p0, p1, p4, p2, Iso_0, Iso_1, Iso_4, Iso_2, intersectP);

                // if M[3] != 0 // cubic equation
                if (M[3] != 0)
                {
                    // cubic
                    numSols = SolveCubic(M, S);
                    for (int i = 0; i < numSols; i++)
                    {

                        // if (S[i] > 0) printf("S[%d]: %.10lf\n", i, S[i]);

                        if (closestIntersection < 0 && S[i] > 0)
                        {
                            closestIntersection = S[i];
                        }

                        if (S[i] > 0 && S[i] < closestIntersection)
                        {
                            closestIntersection = S[i];
                        }
                    }
                    // if (closestIntersection > 0) printf("closestIntersection: %.10lf\n\n", closestIntersection);
                }
                else if (M[2] != 0)
                {
                    // quadratic
                    numSols = SolveQuadric(&M[1], S);
                    for (int i = 0; i < numSols; i++)
                    {

                        // printf("S[%d]: %.10lf\n", i, S[i]);

                        if (closestIntersection < 0 && S[i] > 0)
                        {
                            closestIntersection = S[i];
                        }

                        if (S[i] > 0 && S[i] < closestIntersection)
                        {
                            closestIntersection = S[i];
                        }
                    }
                }
                else
                {
                    // linear
                    if (M[1] != 0)
                    {
                        closestIntersection = -M[0] / M[1];
                    }
                }

                if (closestIntersection < 0)
                {
                    // No intersection
                }
                else
                {
                    hit.t = closestIntersection;
                    hit.position = intersectP + ray.dir * hit.t;
                    hit.material = material;

                    hit.normal.x = A1 * hit.position.y * hit.position.z + B1 * hit.position.y + D1 * hit.position.z + E1;
                    hit.normal.y = A1 * hit.position.x * hit.position.z + B1 * hit.position.x + C1 * hit.position.z + F1;
                    hit.normal.z = A1 * hit.position.x * hit.position.y + C1 * hit.position.y + D1 * hit.position.x + G1;

                    hit.normal.Normalize();

                    // printf("hit.position: %f %f %f\n", hit.position.x, hit.position.y, hit.position.z);

                    /*
                    * Calculate the normal later
                    */
                    //hit.normal = (hit.position - center) / radius;
                }
                break;
            }   // inside the voxel
        }   // t > 0

        if ( i == 0) // top face
        {
            Iso_0 = gridData[2 * (this->index) + 1].z;
            Iso_1 = gridData[2 * (this->index + 1) + 1].z;
            Iso_2 = gridData[2 * (this->index + gridWidth + 1) + 1].z;
            Iso_3 = gridData[2 * (this->index + p) + 1].z;
            Iso_4 = gridData[2 * (this->index + gridWidth) + 1].z;
            Iso_5 = gridData[2 * (this->index + p + gridWidth + 1) + 1].z;
            Iso_6 = gridData[2 * (this->index + p + 1) + 1].z;
            Iso_7 = gridData[2 * (this->index + p + gridWidth) + 1].z;

            // Vector p0, p7
            p0 = gridData[2 * (this->index)];
            p1 = gridData[2 * (this->index + 1)];
            p2 = gridData[2 * (this->index + gridWidth + 1)];
            p4 = gridData[2 * (this->index + gridWidth)];
        }
        else if ( i == 1) // left face
        {
            Iso_0 = gridData[2 * (this->index + p + gridWidth + 1) + 1].z;
            Iso_1 = gridData[2 * (this->index + p) + 1].z;
            Iso_2 = gridData[2 * (this->index + gridWidth + 1) + 1].z;
            Iso_3 = gridData[2 * (this->index + p + gridWidth) + 1].z;
            Iso_4 = gridData[2 * (this->index) + 1].z;
            Iso_5 = gridData[2 * (this->index + gridWidth) + 1].z;
            Iso_6 = gridData[2 * (this->index + p + 1) + 1].z;
            Iso_7 = gridData[2 * (this->index + 1) + 1].z;

            // Vector p0, p7
            p0 = gridData[2 * (this->index + p + gridWidth + 1)];
            p1 = gridData[2 * (this->index + p)];
            p2 = gridData[2 * (this->index + gridWidth + 1)];
            p4 = gridData[2 * (this->index)];
        }
        else if ( i == 2) // right face
        {
            Iso_0 = gridData[2 * (this->index + p + 1) + 1].z;
            Iso_1 = gridData[2 * (this->index + p + gridWidth) + 1].z;
            Iso_2 = gridData[2 * (this->index + 1) + 1].z;
            Iso_3 = gridData[2 * (this->index + p) + 1].z;
            Iso_4 = gridData[2 * (this->index + gridWidth) + 1].z;
            Iso_5 = gridData[2 * (this->index) + 1].z;
            Iso_6 = gridData[2 * (this->index + p + gridWidth + 1) + 1].z;
            Iso_7 = gridData[2 * (this->index + gridWidth + 1) + 1].z;

            // Vector p0, p7
            p0 = gridData[2 * (this->index + p + 1)];
            p1 = gridData[2 * (this->index + p + gridWidth)];
            p2 = gridData[2 * (this->index + 1)];
            p4 = gridData[2 * (this->index + gridWidth)];
        }
        else if ( i == 3) // bottom face
        {
            Iso_0 = gridData[2 * (this->index + p + gridWidth + 1) + 1].z;
            Iso_1 = gridData[2 * (this->index + p + gridWidth) + 1].z;
            Iso_2 = gridData[2 * (this->index + p) + 1].z;
            Iso_3 = gridData[2 * (this->index + gridWidth + 1) + 1].z;
            Iso_4 = gridData[2 * (this->index + p + 1) + 1].z;
            Iso_5 = gridData[2 * (this->index) + 1].z;
            Iso_6 = gridData[2 * (this->index + gridWidth) + 1].z;
            Iso_7 = gridData[2 * (this->index + 1) + 1].z;

            // Vector p0, p7
            p0 = gridData[2 * (this->index + p + gridWidth + 1)];
            p1 = gridData[2 * (this->index + p + gridWidth)];
            p2 = gridData[2 * (this->index + p)];
            p4 = gridData[2 * (this->index + p + 1)];
        }
    } // end of for loop


    closestIntersection = -1;
    // printf("---------------------------------------\n");

    // if (hit.t > 0)
    // {
    //     printf("hit.t: %f\n", hit.t);
    //     printf("hit.position: %f %f %f\n", hit.position.x, hit.position.y, hit.position.z);
    // }

    return hit;
}