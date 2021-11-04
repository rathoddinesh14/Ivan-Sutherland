#ifndef __MARCHINGTETRAHEDRA_H__
#define __MARCHINGTETRAHEDRA_H__

#include <vector>
#include "math_utils.h"
#include "triangulation.h"

namespace marchingtetrahedra
{

    static std::vector<Vector3f> getIsoPoints(int tetraConfig, std::vector<Vector3f> tetrahedraPoints)
    {
        std::vector<Vector3f> isoPoints;

        for (int i = 0; tetraTable[tetraConfig][i] != -1; i += 2)
        {
            Vector3f firstVer = tetrahedraPoints[tetraTable[tetraConfig][i]];
            Vector3f secondVer = tetrahedraPoints[tetraTable[tetraConfig][i + 1]];
            Vector3f midPoint = (firstVer + secondVer) * 0.5f;
            isoPoints.push_back(midPoint);
        }
        return isoPoints;
    }

    static int getTetrahedraConfig(float iso, int x, Vector3f &p0, Vector3f &p1, Vector3f &p2, Vector3f &p3)
    {
        int tetrahedraConfig = 0;

        if (p0.z < iso)
            tetrahedraConfig |= 1;

        if (p1.z < iso)
            tetrahedraConfig |= 2;

        if (p2.z < iso)
            tetrahedraConfig |= 4;

        if (p3.z < iso)
            tetrahedraConfig |= 8;

        return tetrahedraConfig;
    }

    // generate isopoints for given iso-value
    static std::vector<Vector3f> getIsoPoints(float iso, int x, Vector3f *vertices, int w, int h)
    {

        int p = w * h;

        int tetrahedraConfig;

        std::vector<Vector3f> isoPoints, tetrahedraPoints;
        // each cube contains 6 tetrahedra
        // each tetrahedra contains 4 vertices
        // first tetrahedra is formed by vertices 4, 3, 7, 6
        tetrahedraConfig = getTetrahedraConfig(iso, x, vertices[2 * (x + p) + 1], vertices[2 * (x + w) + 1],
                                                  vertices[2 * (x + p + w) + 1], vertices[2 * (x + p + 1 + w) + 1]);
        tetrahedraPoints.push_back(vertices[2 * (x + p)]);
        tetrahedraPoints.push_back(vertices[2 * (x + w)]);
        tetrahedraPoints.push_back(vertices[2 * (x + p + w)]);
        tetrahedraPoints.push_back(vertices[2 * (x + p + 1 + w)]);
        for(auto ele : getIsoPoints(tetrahedraConfig, tetrahedraPoints))
        {
            isoPoints.push_back(ele);
        }
        tetrahedraPoints.clear();

        // second tetrahedra is formed by vertices 4, 3, 6, 2
        tetrahedraConfig = getTetrahedraConfig(iso, x, vertices[2 * (x + p) + 1], vertices[2 * (x + w) + 1],
                                                  vertices[2 * (x + p + 1 + w) + 1], vertices[2 * (x + 1 + w) + 1]);
        tetrahedraPoints.push_back(vertices[2 * (x + p)]);
        tetrahedraPoints.push_back(vertices[2 * (x + w)]);
        tetrahedraPoints.push_back(vertices[2 * (x + p + 1 + w)]);
        tetrahedraPoints.push_back(vertices[2 * (x + 1 + w)]);
        for(auto ele : getIsoPoints(tetrahedraConfig, tetrahedraPoints))
            isoPoints.push_back(ele);           
        tetrahedraPoints.clear();

        // third tetrahedra is formed by vertices 4, 0, 2, 3
        tetrahedraConfig = getTetrahedraConfig(iso, x, vertices[2 * (x + p) + 1], vertices[2 * x + 1],
                                                  vertices[2 * (x + 1 + w) + 1], vertices[2 * (x + w) + 1]);
        tetrahedraPoints.push_back(vertices[2 * (x + p)]);
        tetrahedraPoints.push_back(vertices[2 * x]);
        tetrahedraPoints.push_back(vertices[2 * (x + 1 + w)]);
        tetrahedraPoints.push_back(vertices[2 * (x + w)]);
        for(auto ele : getIsoPoints(tetrahedraConfig, tetrahedraPoints))
            isoPoints.push_back(ele);
        tetrahedraPoints.clear();

        // fourth tetrahedra is formed by vertices 4, 2, 5, 6
        tetrahedraConfig = getTetrahedraConfig(iso, x, vertices[2 * (x + p) + 1], vertices[2 * (x + 1 + w) + 1],
                                                  vertices[2 * (x + p + 1) + 1], vertices[2 * (x + p + w + 1) + 1]);
        tetrahedraPoints.push_back(vertices[2 * (x + p)]);
        tetrahedraPoints.push_back(vertices[2 * (x + 1 + w)]);
        tetrahedraPoints.push_back(vertices[2 * (x + p + 1)]);
        tetrahedraPoints.push_back(vertices[2 * (x + p + w + 1)]);
        for(auto ele : getIsoPoints(tetrahedraConfig, tetrahedraPoints))
            isoPoints.push_back(ele);
        tetrahedraPoints.clear();

        // fifth tetrahedra is formed by vertices 4, 2, 5, 0
        tetrahedraConfig = getTetrahedraConfig(iso, x, vertices[2 * (x + p) + 1], vertices[2 * (x + 1 + w) + 1],
                                                  vertices[2 * (x + p + 1) + 1], vertices[2 * x + 1]);
        tetrahedraPoints.push_back(vertices[2 * (x + p)]);
        tetrahedraPoints.push_back(vertices[2 * (x + 1 + w)]);
        tetrahedraPoints.push_back(vertices[2 * (x + p + 1)]);
        tetrahedraPoints.push_back(vertices[2 * x]);
        for(auto ele : getIsoPoints(tetrahedraConfig, tetrahedraPoints))
            isoPoints.push_back(ele);
        tetrahedraPoints.clear();

        // sixth tetrahedra is formed by vertices 1, 2, 5, 0
        tetrahedraConfig = getTetrahedraConfig(iso, x, vertices[2 * (x + 1) + 1], vertices[2 * (x + 1 + w) + 1],
                                                  vertices[2 * (x + p + 1) + 1], vertices[2 * x + 1]);
        tetrahedraPoints.push_back(vertices[2 * (x + 1)]);
        tetrahedraPoints.push_back(vertices[2 * (x + 1 + w)]);
        tetrahedraPoints.push_back(vertices[2 * (x + p + 1)]);
        tetrahedraPoints.push_back(vertices[2 * x]);
        for(auto ele : getIsoPoints(tetrahedraConfig, tetrahedraPoints))
            isoPoints.push_back(ele);
        tetrahedraPoints.clear();
        
        return isoPoints;
    }

}

#endif // __MARCHINGTETRAHEDRA_H__