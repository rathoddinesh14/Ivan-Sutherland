#include "include/common/lightsource.h"

LightSource::LightSource()
{
    unsigned int indices[] = {
            // square 1
            0, 1, 2,
            2, 3, 0,
            // square 2
            4, 5, 6,
            6, 7, 4,
            // square 3
            0, 4, 7,
            7, 3, 0,
            // square 4
            1, 5, 6,
            6, 2, 1,
            // square 5
            3, 2, 6,
            6, 7, 3,
            // square 6
            4, 0, 3,
            3, 7, 4,
        };

    generateIBO(indices, sizeof(indices) / sizeof(unsigned int));

    program = CompileShaders("shaders/lightsrc/shader.vs", "shaders/lightsrc/shader.fs", nullptr);
    gWorLoc = glGetUniformLocation(program, "gWorld");
}

void LightSource::render(const Matrix4f &VP)
{
    glUseProgram(program);
    Matrix4f trans, scaleMat;
    scaleMat.InitScaleTransform(0.1f, 0.1f, 0.1f);
    trans.InitTranslationTransform(position.x, position.y, position.z);
    MVP = VP * trans * scaleMat;
    glUniformMatrix4fv(gWorLoc, 1, GL_TRUE, &MVP.m[0][0]);
    glBindVertexArray(cubeVAO);
    // draw cube using triangles
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

// setters
void LightSource::setPosition(Vector3f pos)
{
    position = pos;
}

// getters
Vector3f LightSource::getPosition()
{
    return position;
}