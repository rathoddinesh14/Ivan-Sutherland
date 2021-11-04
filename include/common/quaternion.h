#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "math_utils.h"

struct Quaternion
{
    float x, y, z, w;

    Quaternion(float Angle, const Vector3f& V);

    Quaternion(float _x, float _y, float _z, float _w);

    void Normalize();

    Quaternion Conjugate() const;

    Vector3f ToDegrees();
};

Quaternion operator*(const Quaternion& l, const Quaternion& r);

Quaternion operator*(const Quaternion& q, const Vector3f& v);

#endif