#include "include/raytracing/raycam.h"

void RayCamera::set(Vector3f _eye, Vector3f _lookat, Vector3f vup, float _fov, int _windowWidth, int _windowHeight)
{
    eye = _eye;
    lookat = _lookat;
    fov = _fov;
    windowWidth = _windowWidth;
    windowHeight = _windowHeight;
    Vector3f w = eye - lookat;
    float windowSize = w.length() * tanf(fov / 2);
    right = vup.Cross(w).Normalize() * windowSize;
    up = w.Cross(right).Normalize() * windowSize;
}

Ray RayCamera::getRay(int X, int Y)
{
    Vector3f dir = lookat + right * (2 * (X + 0.5f) / windowWidth - 1) + up * (2 * (Y + 0.5f) / windowHeight - 1) - eye;
    return Ray(eye, dir);
}

void RayCamera::Animate(float dt)
{
    Vector3f d = eye - lookat;
    eye = Vector3f(d.x * cos(dt) + d.z * sin(dt), d.y, -d.x * sin(dt) + d.z * cos(dt)) + lookat;
    set(eye, lookat, up, fov, windowWidth, windowHeight);
}
