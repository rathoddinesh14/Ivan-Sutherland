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

#endif
