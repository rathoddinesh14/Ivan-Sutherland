#ifndef __CAMERA_H
#define __CAMERA_H

#pragma once

#include "math_utils.h"
#include <GL/freeglut.h>
#include "quaternion.h"
#include "include/raytracing/rayutils.h"

class Camera
{
    Vector3f pos, tar, Up;

    // camera speed
    float speed;

    Vector2i mousePos;
    bool onLeftEdge, onRightEdge, onTopEdge, onBottomEdge;
    float angleH, angleV;


    int width, height;


    public:
        Camera(int w, int h);
        void rotate(const Vector3f& axis, float angle);
        Matrix4f getMatrix();
        void updatePos(float x, float y, float z);
        void updateTar(float x, float y, float z);

        // get camera pos
        Vector3f getPos() const;

        Ray generateRay(int, int);
        
        // print camera details
        void print();

        // handle keyboard input
        void handleKeyboard(unsigned char key);

        // handle mouse input
        void handleMouse(int x, int y);

        void onRender();

        void update();
};

#endif
