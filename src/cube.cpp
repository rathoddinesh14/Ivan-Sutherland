#include "include/common/cube.h"

Cube::Cube()
{
    float vertices[] = {
        // vertex
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f};

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

    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &cubeIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    program = CompileShaders("shaders/cube.vert", "shaders/cube.frag", nullptr);
    gWorLoc = glGetUniformLocation(program, "MVP");
    MVP.InitIdentity();
}

void Cube::render(const Matrix4f &VP)
{
    // enable wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUseProgram(program);
    Matrix4f trans, scaleMat;
    scaleMat.InitScaleTransform(1.01f, 1.01f, 1.01f);
    // trans.InitTranslationTransform(0.5f, 0.5f, 0.5f);
    MVP = VP * scaleMat;
    glUniformMatrix4fv(gWorLoc, 1, GL_TRUE, &MVP.m[0][0]);
    glBindVertexArray(cubeVAO);
    // draw cube using lines
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);
    // disable wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}