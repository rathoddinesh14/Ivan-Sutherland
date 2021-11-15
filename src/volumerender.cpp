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
    glBufferData(GL_ARRAY_BUFFER, isoPoints.size() * sizeof(Vector3f), &isoPoints[0], GL_STATIC_DRAW);

    // create the vertex array object
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(Vector3f), 0);

    // enable the vertex array object for the normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(Vector3f), (void *)(sizeof(Vector3f)));

    glBindVertexArray(0);

    // clear the data
    delete[] data;

    ShaderProgram = CompileShaders(pVSFileName, pFSFileName, nullptr);
    gWorldLoc = glGetUniformLocation(ShaderProgram, "MVP");
    gIsoVal = glGetUniformLocation(ShaderProgram, "isoValue");
	gWorldTrans = glGetUniformLocation(ShaderProgram, "gWorldTrans");
	gAmbientIntensityLoc = glGetUniformLocation(ShaderProgram, "ambientIntensity");
	gDiffuseIntensityLoc = glGetUniformLocation(ShaderProgram, "diffuseIntensity");
	glightSrcLoc = glGetUniformLocation(ShaderProgram, "lightSrc");
	gSpecLightLoc = glGetUniformLocation(ShaderProgram, "specLight");
	gSpecPowerLoc = glGetUniformLocation(ShaderProgram, "specPower");
	gCamLoc = glGetUniformLocation(ShaderProgram, "camPos");

    updateVBO();
}

void VolumeRender::render(Matrix4f VP)
{
    glUseProgram(ShaderProgram);
    Matrix4f localTrans;
    localTrans.InitIdentity();
    glUniformMatrix4fv(gWorldTrans, 1, GL_TRUE, &localTrans.m[0][0]);
    glUniformMatrix4fv(gWorldLoc, 1, GL_TRUE, &VP.m[0][0]);
    glUniform1f(gIsoVal, isoVal);
    glUniform1f(gAmbientIntensityLoc, ambientLight);
	glUniform1f(gDiffuseIntensityLoc, diffuseLight);
	glUniform1f(gSpecPowerLoc, shininess);
	glUniform1f(gSpecLightLoc, specularLight);
    Vector3f ls = lightSrc->getPosition();
	glUniform3fv(glightSrcLoc, 1, &ls.x);
	glUniform3fv(gCamLoc, 1, &cameraPos.x);

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

void VolumeRender::setLightSrc(LightSource *lightSrc)
{
    this->lightSrc = lightSrc;
}

void VolumeRender::setCameraPos(Vector3f cameraPos)
{
    this->cameraPos = cameraPos;
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
                vector<Vector3f> points = marchingtetrahedra::getIsoPoints(isoVal, index, vertices, width, height);
                for (int l = 0; l < points.size(); l+=3)
                {
                    Vector3f p1 = points[l];
                    Vector3f p2 = points[l + 1];
                    Vector3f p3 = points[l + 2];

                    // compute the normal of the triangle
                    Vector3f normal = (p2 - p1).Cross(p3 - p1);
                    normal.Normalize();

                    // add the vertices to the isoPoints vector
                    isoPoints.push_back(p1);
                    isoPoints.push_back(normal);
                    isoPoints.push_back(p2);
                    isoPoints.push_back(normal);
                    isoPoints.push_back(p3);
                    isoPoints.push_back(normal);
                }
            }
        }
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, isoPoints.size() * sizeof(Vector3f), &isoPoints[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
}