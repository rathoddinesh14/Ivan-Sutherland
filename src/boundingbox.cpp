#include "include/common/boundingbox.h"

BoundingBox::BoundingBox()
{
    unsigned int indices[] = {
        // edge contain 2 vertices
        0, 1,
        1, 2,
        2, 3,
        3, 0,
        4, 5,
        5, 6,
        6, 7,
        7, 4,
        0, 4,
        1, 5,
        2, 6,
        3, 7};

    generateIBO(indices, sizeof(indices) / sizeof(unsigned int));

    program = CompileShaders("shaders/cube.vert", "shaders/cube.frag", nullptr);
    gWorLoc = glGetUniformLocation(program, "MVP");
}

void BoundingBox::render(const Matrix4f &VP)
{
    glUseProgram(program);
    Matrix4f trans, scaleMat;
    scaleMat.InitScaleTransform(1.01f, 1.01f, 1.01f);
    trans.InitTranslationTransform(0.5f, 0.5f, 0.5f);
    MVP = VP * trans * scaleMat;
    glUniformMatrix4fv(gWorLoc, 1, GL_TRUE, &MVP.m[0][0]);
    glBindVertexArray(cubeVAO);
    // draw cube using lines
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}