#ifndef __SCENE_H
#define __SCENE_H

#include "include/common/math_utils.h"
#include "include/common/camera.h"
#include "rayutils.h"
#include "include/common/sphere.h"

#pragma once

#define ITERATION_DEPTH 5

const float epsilon = 0.0001f;

struct Light
{
	Vector3f direction;
	Vector3f emittence;

	Light(Vector3f _direction, Vector3f _Le) {
		direction = _direction.Normalize();
		emittence = _Le;
	}
};

class Scene
{
	std::vector<Intersectable*> objects;
	std::vector<Light*> lights;
	Camera* cam;
	Vector3f ambientLight;

	public:
		Scene(Camera *camera, Vector3f ambientLight);
		bool shadowIntersect(Ray ray);
		void addObject(Intersectable*object);
		void render(std::vector<Vector4f>& image, int height, int width);
		Vector3f trace(Ray ray, int depth);
		Hit intersect(Ray ray);
		void Animate(float dt);
};

#endif