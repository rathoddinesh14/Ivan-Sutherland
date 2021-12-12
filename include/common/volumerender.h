#ifndef __VOLUMERENDER_H
#define __VOLUMERENDER_H

#include <GL/glew.h>
#include <vector>
#include "math_utils.h"
#include "shader.h"
#include "rawfileparser.h"
#include "marchingtetrahedra.h"
#include "lightsource.h"
#include "domainsearch.h"
#include "chrono"
#include "raycasting.h"

class RaycastingRender;

class VolumeRender
{
    GLuint VAO, VBO, ibo, ShaderProgram;
    int width, height, depth;
    float minVal, maxVal;
    GLuint gWorldLoc, gIsoVal;
    GLuint iboXY = 0, iboXZ = 0, iboYZ = 0;
    const char *pVSFileName = "shaders/marching tetrahedra/shader.vs";
    const char *pFSFileName = "shaders/marching tetrahedra/shader.fs";
    float isoVal, stepSize;
    int moveZ = 0;
    int moveX = 0;
    int moveY = 0;
    Vector3f *vertices;
    vector<Vector3f> isoPoints;
    GLuint gAmbientIntensityLoc, glightSrcLoc, gDiffuseIntensityLoc, gSpecLightLoc, gSpecPowerLoc, gWorldTrans, gCamLoc;
    float ambientLight = 0.3f, diffuseLight = 0.0f, specularLight = 0.5f, shininess = 32.0f;
    Vector3f cameraPos;
    LightSource *lightSrc;
    DomainSearch *domainSearch;
    RaycastingRender *raycastingRender;
    int algo = 0;   // marching tetrahedra is default
                    // 1 - domain search
                    // 2 - raycasting


public:
    VolumeRender(char *rawFile);
    ~VolumeRender();

    // create ibo for slice
    void createIBO(char axis, int slice);

    void render(Matrix4f VP, Matrix4f Model);

    void handleKeyboard(unsigned char key);

    void setIsoValue(float val);
    float getIsoValue() const;

    float getMinVal() const;
    float getMaxVal() const;

    void setAlgo(int algo);
    int getAlgo() const;

    // after getting new iso points from marching cube, update the ibo
    void updateVBO();

    void setLightSrc(LightSource *lightSrc);

    void setCameraPos(Vector3f cameraPos);

    // set raycasting render
    void setRaycastingRender(RaycastingRender *raycastingRender);
};

#endif