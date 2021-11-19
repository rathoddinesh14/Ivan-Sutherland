#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "include/raytracing/rayutils.h"
#include "include/common/math_utils.h"

class Sphere : public Intersectable {
	Vector3f center;
	float radius;
public:
	Sphere(const Vector3f& _center, float _radius, Material* _material) {
		center = _center; radius = _radius; material = _material;
	}

	Hit intersect(const Ray& ray) {
		Hit hit;
		Vector3f dist = ray.start - center;
		float a = ray.dir.Dot(ray.dir);
		float b = dist.Dot(ray.dir) * 2.0f;
		float c = dist.Dot(dist) - radius * radius;
		float discr = b * b - 4.0f * a * c;
		if (discr < 0) return hit;
		float sqrt_discr = sqrtf(discr);
		float t1 = (-b + sqrt_discr) / 2.0f / a;	// t1 >= t2 for sure
		float t2 = (-b - sqrt_discr) / 2.0f / a;
		if (t1 <= 0) return hit;
		hit.t = (t2 > 0) ? t2 : t1;
		hit.position = ray.start + ray.dir * hit.t;
		hit.normal = (hit.position - center) * ( 1.0 / radius);
		hit.material = material;
		return hit;
	}
};
#endif // __SPHERE_H__