#ifndef __RAYCASTING_H__
#define __RAYCASTING_H__

#include <GL/glew.h>
#include "shader.h"
#include "include/common/camera.h"
#include "include/common/volumerender.h"

class RaycastingRender
{

    GLuint vbo, vao, ibo, ShaderProgram;
    const char *pVSFileName = "shaders/ray casting/raycast.vs";
    const char *pFSFileName = "shaders/ray casting/raycast.fs";
    Camera *camera;
    GLuint gWorldLoc, gCamLoc;
    GLuint volumeTex, volumeLoc, isoValueLoc;
    float isoValue;
    GLuint stepSizeLoc;
    Vector3f stepSize;
    int width, height, depth;

public:
    RaycastingRender(Camera *camera, char *rawFile);
    void render(Matrix4f VP, Matrix4f Model);

    // Getters and Setters
    void setIsoValue(float value);

    ~RaycastingRender();
};

#endif // __RAYCASTING_H__