#include "include/raytracing/render.h"

RayTraceRender::RayTraceRender(int width, int height)
{
    this->width = width;
    this->height = height;

    // generate vao
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    // generate vbo
    GLuint vbo;
    glGenBuffers(1, &vbo);

    // bind vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    // set vertex data - 2 triangles forming quad
    float vertices[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f,
        -1.0f, 1.0f
    };

    // compile shader
    this->program = CompileShaders("shaders/ray tracing/raytrace.vert", "shaders/ray tracing/raytrace.frag", NULL);
    glUseProgram(this->program);
    this->u_textureUnit = glGetUniformLocation(this->program, "textureUnit");

    // set vertex data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set vertex attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    // generate texture
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

RayTraceRender::~RayTraceRender()
{
    // delete vao
    glDeleteVertexArrays(1, &this->vao);

    // delete texture
    glDeleteTextures(1, &this->texture);
}

void RayTraceRender::render()
{
    // bind vao
    glBindVertexArray(this->vao);

    // activate texture
    const GLuint textureUnit = 0;
    if (this->u_textureUnit >= 0)
    {
        glUniform1i(this->u_textureUnit, textureUnit);
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, this->texture);
    }

    // draw triangles forming quad
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void RayTraceRender::loadTexture(std::vector<Vector4f> &image)
{
    // bind texture
    glBindTexture(GL_TEXTURE_2D, this->texture);

    // set texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_FLOAT, &image[0]);
}