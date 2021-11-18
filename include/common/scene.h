#ifndef __SCENE_H
#define __SCENE_H

#pragma once

#define ITERATION_DEPTH 5

struct Light {
	Vector3f direction;
	Vector3f emittence;
	Light(Vector3f _direction, Vector3f _Le) {
		direction = _direction.Normalize();
		emittence = _Le;
	}
};

class Scene{
	std::vector<Intersectable*> objects;
	std::vector<Light*>lights;
	Camera camera;
	Vector3f ambientLight;
	public:
		Scene(Camera camera, Vector3f ambientLight);
		void addObject(Intersectable*object);
		void render(std::vector<Vector4f>& image);
		Vector3f trace(Ray ray, int depth);
		Hit intersect(Ray ray);
};

#endif
