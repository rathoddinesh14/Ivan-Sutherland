#include "include/common/raycasting.h"

RaycastingRender::RaycastingRender(Camera *camera, char *rawFile)
{
    // Load the volume texture
    GLubyte *volumeData = loadRawDataInByte(rawFile, width, height, depth);

    stepSize.x = 1.0f / width;
    stepSize.y = 1.0f / height;
    stepSize.z = 1.0f / depth;

    // Create the volume texture
    glGenTextures(1, &volumeTex);
    glBindTexture(GL_TEXTURE_3D, volumeTex);

    // set the texture parameters
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //set the mipmap levels (base and max)
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 4);

    // Set the texture data
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, width, height, depth, 0, GL_RED, GL_UNSIGNED_BYTE, volumeData);

    // generate the mipmap
    glGenerateMipmap(GL_TEXTURE_3D);

    // Free the volume data
    delete[] volumeData;

    // // print in green
    // std::cout << "\033[0;32m"
    //           << "Volume texture loaded successfully"
    //           << "\033[0m" << std::endl;

    this->camera = camera;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

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

    //unit cube indices
    GLushort indices[36] = {0, 5, 4,
                            5, 0, 1,
                            3, 7, 6,
                            3, 6, 2,
                            7, 4, 6,
                            6, 4, 5,
                            2, 1, 3,
                            3, 1, 0,
                            3, 0, 7,
                            7, 0, 4,
                            6, 5, 2,
                            2, 5, 1};

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

    //enable vertex attributre array for position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //pass indices to element array  buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);

    ShaderProgram = CompileShaders(pVSFileName, pFSFileName, nullptr);
    glUseProgram(ShaderProgram);
    gWorldLoc = glGetUniformLocation(ShaderProgram, "MVP");
    gCamLoc = glGetUniformLocation(ShaderProgram, "cameraPos");
    volumeLoc = glGetUniformLocation(ShaderProgram, "volumeTexture");
    isoValueLoc = glGetUniformLocation(ShaderProgram, "isoValue");
    stepSizeLoc = glGetUniformLocation(ShaderProgram, "rayStepSize");
    glUniform1i(volumeLoc, 0);
}

void RaycastingRender::render(Matrix4f VP, Matrix4f Model)
{
    glUseProgram(ShaderProgram);
    glEnable(GL_BLEND);
    glBindVertexArray(vao);
    //get the camera position
    Vector3f camPosition = camera->getPos();
    // Matrix4f trans;
    // trans.InitIdentity();
    // // trans.InitTranslationTransform(-0.5f, -0.5f, -0.5f);
    // Model = Model * trans;
    // trans = VP * trans;
    // glUniformMatrix4fv(gWorldTrans, 1, GL_TRUE, &Model.m[0][0]);
    // glUniformMatrix4fv(gWorldLoc, 1, GL_TRUE, &trans.m[0][0]);

    Matrix4f trans;
    trans.InitIdentity();
    // trans.InitTranslationTransform(-0.5f, -0.5f, -0.5f);
    trans = VP * trans;
    Matrix4f undoTrans = Model.Inverse();
    Vector4f originalCam = Vector4f(camPosition.x, camPosition.y, camPosition.z, 1.0);
    originalCam = undoTrans * originalCam;
    camPosition.x = originalCam.x;
    camPosition.y = originalCam.y;
    camPosition.z = originalCam.z;
    glUniformMatrix4fv(gWorldLoc, 1, GL_TRUE, &trans.m[0][0]);
    glUniform3fv(gCamLoc, 1, &camPosition.x);
    glUniform3f(stepSizeLoc, stepSize.x, stepSize.y, stepSize.z);
    glUniform1i(volumeLoc, 0);
    // pass float isovalue to shader
    glUniform1f(isoValueLoc, isoValue);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    glUseProgram(0);
    glDisable(GL_BLEND);
}

void RaycastingRender::setIsoValue(float value) 
{
    this->isoValue = value;
}

RaycastingRender::~RaycastingRender()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);
    glDeleteTextures(1, &volumeTex);
    glDeleteProgram(ShaderProgram);
}