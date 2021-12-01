#ifndef __INTERSECTABLE_H
#define __INTERSECTABLE_H

#include "include/common/camera.h"
#pragma once

enum MaterialType {ROUGH, REFLECTIVE, REFRACTIVE};

struct Material {
	Vector3f ka, kd, ks;
	float  shininess;
	Vector3f F0;
	float ior;
	MaterialType type;
	Material(MaterialType t) { type = t; }
};

struct RoughMaterial : Material {
	RoughMaterial(Vector3f _kd, Vector3f _ks, float _shininess) : Material(ROUGH) {
		ka = _kd * M_PI; 
		kd = _kd;
		ks = _ks;
		shininess = _shininess;
	}
};

struct ReflectiveMaterial : Material {
	ReflectiveMaterial(Vector3f n, Vector3f kappa) : Material(REFLECTIVE) {
		Vector3f one(1, 1, 1);
		F0 = ((n - one) * (n - one) + kappa * kappa) / ((n + one)*(n + one) + kappa * kappa);
	}
};

struct RefractiveMaterial : Material {
	RefractiveMaterial(Vector3f n) : Material(REFRACTIVE) {
		Vector3f one(1, 1, 1);
		F0 = ((n - one)*(n - one)) / ((n + one)*(n + one));
		ior = n.x;
	}
};

/**
 * @brief result of ray and surface intersection
 * 
 */
struct Hit {

	// ray parameter of intersection a + b*t
	float t;

	Vector3f position, normal;
	Material * material;
	
	Hit() {
		t = -1;
	}
};

/**
 * @brief Ray structure
 * 
 */
struct Ray {
	
	Vector3f start, dir;

	Ray(Vector3f _start, Vector3f _dir) {
		start = _start;
		dir = _dir.Normalize();
	}
};

/**
 * @brief All intersectable objects should extend this base class
 * 
 */
class Intersectable {
	protected:
		Material * material;
	public:
		virtual Hit intersect(const Ray& ray) = 0;
};


/**
 * @brief Get the Plane equation
 * c[3] - x, c[2] - y, c[1] - z, c[0] - d
 * @param c - coefficients of the equation
 * @param pt - point on the plane
 * @param DC - Direction cosines
 * @return void
 */
static void GetPlane(double c[4], Vector3f pt, Vector3f DC)
{
    c[3] = DC.x;
    c[2] = DC.y;
    c[1] = DC.z;
    c[0] = -(DC.x * pt.x + DC.y * pt.y + DC.z * pt.z);
}

/**
 * @brief function to check if a point is inside the triangle
 * 
 * @param A, B, C - vertices of the triangle, P - point to be checked
 * @return bool - true if the point is inside the triangle
 */
static bool IsInsideTriangle(Vector3f A, Vector3f B, Vector3f C, Vector3f P)
{
    A = A - P;
    B = B - P;
    C = C - P;

    // u - PBC, v - PCA, w - PAB

    Vector3f u = B.Cross(C);
    Vector3f v = C.Cross(A);
    Vector3f w = A.Cross(B);

    std::cout << std::flush;

    if (u.Dot(v) < 0.0)
        return false;
    if (u.Dot(w) < 0.0)
        return false;
    
    // all normals point in the same direction
    return true;
}

/**
 * @brief isovalue of a point using bilinear interpolation
 * 
 * @param A, B, C, D - vertices of the polygon, P - point to be checked
 * @param AVal, BVal, CVal, DVal - values of the vertices
 * @return float - isovalue of the point
 */
static float GetIsoValue(Vector3f A, Vector3f B, Vector3f C, Vector3f D, float AVal, float BVal, float CVal, float DVal, Vector3f P)
{
    float R1 = ((B.x - P.x) / (B.x - A.x)) * AVal + ((P.x - A.x) / (B.x - A.x)) * BVal;
    float R2 = ((C.x - P.x) / (C.x - D.x)) * DVal + ((P.x - D.x) / (C.x - D.x)) * CVal;
    return ((P.y - B.y)/(C.y - B.y)) * R2 + ((C.y - P.y)/(C.y - B.y)) * R1;
}

/**
 * @brief ray intersects plane
 * 
 * @param ray 
 * @param tl 
 * @param tr 
 * @param bl 
 * @param br 
 * @param DC 
 * @return true 
 * @return false 
 */
static bool rayPlaneIntersect(Ray &ray, Vector3f tl, Vector3f tr, Vector3f bl, Vector3f br, Vector3f DC)
{
    // std::cout << "rayPlaneIntersect function" << std::endl;

	double plane[4];
    GetPlane(plane, tl, DC);
    float num = - (plane[3] * ray.start.x + plane[2] * ray.start.y + plane[1] * ray.start.z + plane[0]);
    float denom = plane[3] * ray.dir.x + plane[2] * ray.dir.y + plane[1] * ray.dir.z;
    
	if (denom != 0.0f)
    {
        float t = num / denom;
        if (t < 0)
            return false;
        else
        {
            // std::cout << "t: " << t << std::endl;

            Vector3f intersectP = ray.start + ray.dir * t;
            
            // check if the intersection point is inside the triangle
            if (IsInsideTriangle(bl, br, tr, intersectP) || IsInsideTriangle(bl, tr, tl, intersectP))
            {
                // std::cout << "intersectP: " << std::endl;
                return true;
            }
        }
    }
    else return false;
}

#endif