#ifndef __SCENE_H
#define __SCENE_H

#include "include/common/volumerender.h"
#include "include/common/math_utils.h"
#include "rayutils.h"
#include "include/common/sphere.h"
#include "raycam.h"
#include "domainsearch.h"

#pragma once

#define ITERATION_DEPTH 1

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

class VolumeRender;

class Scene
{
	std::vector<Intersectable*> objects;
	std::vector<Light*> lights;
	RayCamera *camera;
	Vector3f ambientLight;
	VolumeRender *volumeRenderer;

	int width;
	int height;

	float isoValue;

	public:
		Scene(Vector3f ambientLight, int width, int height, VolumeRender *vr);
		bool shadowIntersect(Ray ray);
		void addObject(Intersectable*object);
		void render(std::vector<Vector4f> *image);
		Vector3f trace(Ray ray, int depth);
		Hit intersect(Ray ray);
		void Animate(float dt);

		// setters
		void setIsoValue(float iso);

		// getters
		float getIsoValue();
};

#endif