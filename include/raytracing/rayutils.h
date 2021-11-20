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

#endif
