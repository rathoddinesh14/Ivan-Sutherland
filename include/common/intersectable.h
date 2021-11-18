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

struct Hit {
	float t;
	Vector3f position, normal;
	Material * material;
	Hit() {
		t = -1;
	}
};

class Intersectable{
	protected:
		Material * material;
	public:
		virtual Hit intersect(const Ray& ray) = 0;
};

#endif
