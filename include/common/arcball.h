#ifndef __ARCBALL_H__
#define __ARCBALL_H__

#include "math_utils.h"
#include <GL/freeglut.h>
#include <iostream>

/**
 * @brief Arcball class
 * @ author Dinesh Rathod
 * 
 */
class ArcBall
{

    // window width and height
    int width, height;

    // Arcball rotation speed
    GLfloat speed;

    // session starts/end with mouse left button down
    bool session;

    // mouse tracking
    bool tracking;

    // angle
    GLfloat angle;

    // last mouse position
    Vector3f last_mouse_pos;

    // camera axis
    Vector3f camAxis;

    public:
        ArcBall(int width, int height, GLfloat speed);
        ~ArcBall();
        Vector3f getNormalizedScreenCoord(int x, int y);
        void handleMouseButton(int button, int state, int x, int y);
        void handleCursor(int x, int y);
        void updateWindowSize(int width, int height);

        // returns rotation matrix for the current arcball
        Matrix4f getRotationMatrix();
};

#endif // __ARCBALL_H__