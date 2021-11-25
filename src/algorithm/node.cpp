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

    // printf("Iso_0: %f\n", Iso_0);
    // gridData[2 * (this->index + p)].Print();
    // printf("\n");
    // printf("Iso_1: %f\n", Iso_1);
    // gridData[2 * (this->index + p + 1)].Print();
    // printf("\n");
    // printf("Iso_2: %f\n", Iso_2);
    // gridData[2 * (this->index)].Print();
    // printf("\n");
    // printf("Iso_3: %f\n", Iso_3);
    // gridData[2 * (this->index + p + gridWidth)].Print();
    // printf("\n");
    // printf("Iso_4: %f\n", Iso_4);
    // gridData[2 * (this->index + 1)].Print();
    // printf("\n");
    // printf("Iso_5: %f\n", Iso_5);
    // gridData[2 * (this->index + gridWidth)].Print();
    // printf("\n");
    // printf("Iso_6: %f\n", Iso_6);
    // gridData[2 * (this->index + p + gridWidth + 1)].Print();
    // printf("\n");
    // printf("Iso_7: %f\n", Iso_7);
    // gridData[2 * (this->index + gridWidth + 1)].Print();
    // printf("\n");

    // print index
    // printf("index: %d\n", this->index);

    // Vector p0, p7
    p0 = gridData[2 * (this->index + p)];
    p1 = gridData[2 * (this->index + p + 1)];
    p2 = gridData[2 * (this->index)];
    p4 = gridData[2 * (this->index + 1)];

    // printf("p0: %f %f %f\n", p0.x, p0.y, p0.z);
    
    // plane equation
    GetPlane(plane, p0, Vector3f(0.0f, 0.0f, 1.0f));

    // printf("plane: %f %f %f %f\n", plane[0], plane[1], plane[2], plane[3]);


    // finding intersection
    float t = - (plane[3] * ray.start.x + plane[2] * ray.start.y + plane[1] * ray.start.z + plane[0]) /
                     (plane[3] * ray.dir.x + plane[2] * ray.dir.y + plane[1] * ray.dir.z);

    // printf("t = %f\n", t);
    // t > 0 intersection point
    if (t > 0)
    {
        // intersection point
        intersectP = ray.start + ray.dir * t;

        // check if inside the voxel
        if (IsInsideTriangle(p0, p1, p4, intersectP) || IsInsideTriangle(p0, p4, p2, intersectP))
        {
            // printf("intersectP: %f %f %f\n", intersectP.x, intersectP.y, intersectP.z);
            // printf("iso value %f\n", GetIsoValue(p0, p1, p4, p2, Iso_0, Iso_1, Iso_4, Iso_2, intersectP));

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

            // printf("M: %f %f %f %f\n", M[0], M[1], M[2], M[3]);

            // if M[3] != 0 // cubic equation
            if (M[3] != 0)
            {
                // cubic
                numSols = SolveCubic(M, S);
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

                // printf("hit.position: %f %f %f\n", hit.position.x, hit.position.y, hit.position.z);

                /*
                * Calculate the normal later
                */
                //hit.normal = (hit.position - center) / radius;
            }

        }   // inside the voxel
    }   // t > 0

    closestIntersection = -1;
    // printf("---------------------------------------\n");
    return hit;
}