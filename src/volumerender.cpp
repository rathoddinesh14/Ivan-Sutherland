#include "include/common/volumerender.h"

VolumeRender::VolumeRender(char *rawFile)
{
    glGenVertexArrays(1, &VAO);
    cout << "VAO: " << VAO << endl;
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    float *data = loadRawData(rawFile, width, height, depth, minVal, maxVal);
    // width = 3;
    // height = 3;
    // depth = 3;
    // minVal = 4;
    // maxVal = 14;
    // float data[] = {4, 5, 6, 6, 5, 7, 7, 5, 8, 8, 5, 9, 9, 5, 10, 10, 5, 11, 11, 5, 12, 12, 5, 13, 13, 5, 14};

    domainSearch = new DomainSearch(width, height, depth, data);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(Vector3f), 0);

    // // enable the vertex array object for the normals
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(Vector3f), (void *)(sizeof(Vector3f)));

    glBindVertexArray(0);

    // clear the data
    delete[] data;

    ShaderProgram = CompileShaders(pVSFileName, pFSFileName, nullptr);
    gWorldLoc = glGetUniformLocation(ShaderProgram, "MVP");
    gIsoVal = glGetUniformLocation(ShaderProgram, "isoValue");
    // gWorldTrans = glGetUniformLocation(ShaderProgram, "gWorldTrans");
    // gAmbientIntensityLoc = glGetUniformLocation(ShaderProgram, "ambientIntensity");
    // gDiffuseIntensityLoc = glGetUniformLocation(ShaderProgram, "diffuseIntensity");
    // glightSrcLoc = glGetUniformLocation(ShaderProgram, "lightSrc");
    // gSpecLightLoc = glGetUniformLocation(ShaderProgram, "specLight");
    // gSpecPowerLoc = glGetUniformLocation(ShaderProgram, "specPower");
    // gCamLoc = glGetUniformLocation(ShaderProgram, "camPos");

    updateVBO();
}

VolumeRender::~VolumeRender()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(ShaderProgram);
    delete domainSearch;
    delete[] vertices;
}

void VolumeRender::render(Matrix4f VP, Matrix4f Model)
{

    if (algo == 2)
    {
        // ray casting
        this->raycastingRender->render(VP, Model);
    }
    else
    {
        glUseProgram(ShaderProgram);
        Matrix4f trans;
        trans.InitIdentity();
        trans.InitTranslationTransform(-0.5f, -0.5f, -0.5f);
        trans = VP * trans;
        // Model = Model * trans;
        // glUniformMatrix4fv(gWorldTrans, 1, GL_TRUE, &Model.m[0][0]);
        glUniformMatrix4fv(gWorldLoc, 1, GL_TRUE, &trans.m[0][0]);
        // glUniform1f(gIsoVal, isoVal);
        // glUniform1f(gAmbientIntensityLoc, ambientLight);
        // glUniform1f(gDiffuseIntensityLoc, diffuseLight);
        // glUniform1f(gSpecPowerLoc, shininess);
        // glUniform1f(gSpecLightLoc, specularLight);
        // Vector3f ls = lightSrc->getPosition();
        // glUniform3fv(glightSrcLoc, 1, &ls.x);
        // glUniform3fv(gCamLoc, 1, &cameraPos.x);

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
}

void VolumeRender::handleKeyboard(unsigned char key)
{
}

void VolumeRender::setIsoValue(float val)
{
    this->isoVal = val;
    this->raycastingRender->setIsoValue(val/this->maxVal);
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

void VolumeRender::setAlgo(int algo)
{
    this->algo = algo;
}

int VolumeRender::getAlgo() const
{
    return this->algo;
}

void VolumeRender::updateVBO()
{
    isoPoints.clear();
    // calculate the execution time
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    if (algo == 0)
    {
        // marching tetrahedra
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
                    }
                }
            }
        }
    }
    else
    {
        // domain search
        // get nodes in the octree
        vector<Node *> nodes = domainSearch->getNodes(isoVal);
        for (auto node : nodes)
        {
            for (auto p : marchingtetrahedra::getIsoPoints(isoVal, node->index, vertices, width, height))
            {
                isoPoints.push_back(p);
            }
        }
    }
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(end - begin);
    std::cout << "execution time: " << time_span.count() * 1000 << " ms" << std::endl;

    // print number of iso points
    std::cout << "isoPoints size: " << isoPoints.size() << std::endl;

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, isoPoints.size() * sizeof(Vector3f), &isoPoints[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void VolumeRender::setRaycastingRender(RaycastingRender *raycastingRender)
{
    this->raycastingRender = raycastingRender;
}