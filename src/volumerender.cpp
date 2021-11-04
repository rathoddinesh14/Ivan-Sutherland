#include "include/common/volumerender.h"

VolumeRender::VolumeRender(char *rawFile)
{
    glGenVertexArrays(1, &VAO);
    cout << "VAO: " << VAO << endl;
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    float *data = loadRawData(rawFile, width, height, depth, minVal, maxVal);

    isoVal = minVal;
    stepSize = (maxVal - minVal) / 100;
    vertices = new Vector3f[2 * depth * width * height];
    unsigned int *indices = new unsigned int[depth * width * height];

    // initialize vertices with unit cube spread over the whole volume
    for (int i = 0; i < depth; i++)
    {
        for (int j = 0; j < height; j++)
        {
            for (int k = 0; k < width; k++)
            {
                // normalise the coordinates to fit in the unit cube
                float x = (float)k / (width - 1);
                float y = (float)j / (height - 1);
                float z = (float)(depth - 1 - i) / (depth - 1);
                int index = i * width * height + j * width + k;
                vertices[2 * index] = Vector3f(x, z, y);
                vertices[2 * index + 1] = Vector3f(data[index]);
                indices[index] = 2 * index;
            }
        }
    }

    // create the vertex buffer object
    glBufferData(GL_ARRAY_BUFFER, isoPoints.size() * sizeof(Vector3f), &isoPoints[0], GL_DYNAMIC_DRAW);

    // create the vertex array object
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);

    // clear the data
    delete[] data;

    ShaderProgram = CompileShaders(pVSFileName, pFSFileName, nullptr);
    gWorldLoc = glGetUniformLocation(ShaderProgram, "MVP");
    gIsoVal = glGetUniformLocation(ShaderProgram, "isoValue");

    updateVBO();
}

void VolumeRender::render(Matrix4f VP)
{
    glUseProgram(ShaderProgram);
    glUniformMatrix4fv(gWorldLoc, 1, GL_TRUE, &VP.m[0][0]);
    glUniform1f(gIsoVal, isoVal);
    glBindVertexArray(VAO);
    // wire frame
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // render using points
    // point size is set to 5
    // glPointSize(5);
    // cout << "isoPoints size: " << isoPoints.size() << endl;
    glDrawArrays(GL_TRIANGLES, 0, isoPoints.size());
    // glDrawElements(GL_POINTS, depth * width * height, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glUseProgram(0);
}

void VolumeRender::handleKeyboard(unsigned char key)
{
}

void VolumeRender::setIsoValue(float val) 
{
    this->isoVal = val;
}

float VolumeRender::getIsoValue() const
{
    return this->isoVal;
}

float VolumeRender::getMinVal() const
{
    return this->minVal;
}

float VolumeRender::getMaxVal() const
{
    return this->maxVal;
}

void VolumeRender::updateVBO()
{
    isoPoints.clear();
    for (int i = 0; i < depth - 1; i++)
    {
        for (int j = 0; j < height - 1; j++)
        {
            for (int k = 0; k < width - 1; k++)
            {
                int index = i * width * height + j * width + k;

                for (auto p : marchingtetrahedra::getIsoPoints(isoVal, index, vertices, width, height))
                {
                    isoPoints.push_back(p);
                    // p.Print();
                }
            }
        }
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, isoPoints.size() * sizeof(Vector3f), &isoPoints[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
}