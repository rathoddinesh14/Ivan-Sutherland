#ifndef __CUBE_H_
#define __CUBE_H_

#include <GL/glew.h>
#include "shader.h"
#include "math_utils.h"

struct Cube
{
    // first, configure the cube's VAO (and VBO)
    unsigned int cubeIBO, cubeVBO, cubeVAO;
    // shader program
    GLuint program, gWorLoc;

    Matrix4f MVP;

    Cube();

    void generateIBO(unsigned int indices[], unsigned long size);

    virtual void render(const Matrix4f &VP) {}
};

#endif