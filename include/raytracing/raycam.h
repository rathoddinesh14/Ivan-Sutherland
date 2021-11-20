#ifndef __RAYCAM_H__
#define __RAYCAM_H__

#include "include/common/math_utils.h"
#include "rayutils.h"

class RayCamera {
	Vector3f eye, lookat, right, up;
	float fov;
    int windowWidth, windowHeight;

public:
	void set(Vector3f _eye, Vector3f _lookat, Vector3f vup, float _fov, int _windowWidth, int _windowHeight);

	Ray getRay(int X, int Y);
	void Animate(float dt);
};
#endif // __RAYCAM_H__