// rawfileparser.h
/*NOTE: ensure that the filenames are in the following format
<name>_<XxYxZ>_<format>.raw
<name> shouldn't have '_','x','.' 
Absolutely no error-checking has been implemented so be careful about the filename

The code is compliant to c++14, in Linux variants while compling with g++, add the flag -std=c++14. 
If compilation errors occur due to lack of support for c++14, modify the code accordingly.
*/

#ifndef __RAWFILEPARSER_H
#define __RAWFILEPARSER_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <GL/glew.h>

using namespace std;

/*helper function to split input filename, source: stackoverflow*/
static vector<string> split(const string &s, char delim)
{
    vector<string> result;
    stringstream ss(s);
    string item;

    while (getline(ss, item, delim))
    {
        result.push_back(item);
    }

    return result;
}

static GLubyte *loadRawDataInByte(const char *filepath, int &width, int &height, int &depth)
{
    cout << "Loading raw file: " << filepath << endl;

    //parse each argument separately
    string sourceFile(filepath);

    // remove path from source file name
    size_t found = sourceFile.find_last_of("/\\");
    string sourceFileName = sourceFile.substr(found + 1);

    found = sourceFileName.find_last_of("_");
    vector<string> params = split(sourceFileName, '_');

    string xyzext = params[1];
    string formatspec = params[2];

    //remove the .raw extension
    vector<string> format = split(formatspec, '.');

    //split the xyz extants using 'x' as delimiter and convert them to integers
    vector<string> xyz = split(xyzext, 'x');

    width = stoi(xyz[0]);
    height = stoi(xyz[1]);
    depth = stoi(xyz[2]);

    cout << "Dimensions of the image: " << width << "x" << height << "x" << depth << endl;

    std::ifstream infile(filepath, std::ios_base::binary);

    if (!infile.good())
    {
        // print error message
        std::cout << "Error: could not open file " << filepath << std::endl;
        exit(1);
    }

    GLubyte *data = new GLubyte[width * height * depth];

    infile.read(reinterpret_cast<char *>(data), width * height * depth * sizeof(GLubyte));
    infile.close();
    
    return data;
}

static float *loadRawData(const char *filepath, int &width, int &height, int &depth, float &minVal, float &maxVal)
{
    cout << "Loading raw file: " << filepath << endl;

    //parse each argument separately
    string sourceFile(filepath);

    // remove path from source file name
    size_t found = sourceFile.find_last_of("/\\");
    string sourceFileName = sourceFile.substr(found + 1);

    found = sourceFileName.find_last_of("_");
    vector<string> params = split(sourceFileName, '_');

    string xyzext = params[1];
    string formatspec = params[2];

    //remove the .raw extension
    vector<string> format = split(formatspec, '.');

    //split the xyz extants using 'x' as delimiter and convert them to integers
    vector<string> xyz = split(xyzext, 'x');

    width = stoi(xyz[0]);
    height = stoi(xyz[1]);
    depth = stoi(xyz[2]);

    cout << "Dimensions of the image: " << width << "x" << height << "x" << depth << endl;

    //calculate the size of the data
    int size = width * height * depth;

    ifstream infile;

    //open the raw file to read it's contents
    infile.open(sourceFile, ios::binary | ios::in);

    // if the file is not open, print an error message
    if (!infile.is_open())
    {
        cout << "\033[1;31mError: could not open file " << sourceFile<< endl;
        cout << "or may be provided wrong file location\033[0m" << endl;

        exit(1);
    }

    char *datac;
    int *datai;
    if (format[0] == "uint8")
    {
        datac = new char[size];
        infile.read(datac, size);
    }
    else if (format[0] == "uint16")
    {
        datai = new int[size];
        infile.read((char *)datai, size);
    }

    infile.close();

    //allocate memory for the data
    float *data = new float[size];

    minVal = FLT_MAX;
    maxVal = -FLT_MAX;
    for (auto i = 0; i < size; i++)
    {
        if (format[0] == "uint8")
        {
            data[i] = datac[i];
            if (data[i] < 0)
                data[i] = data[i] + 256;
        }
        else if (format[0] == "uint16")
        {
            data[i] = datai[i];
            if (data[i] < 0)
                data[i] = data[i] + 65536;
        }
        if (data[i] < minVal)
            minVal = data[i];
        if (data[i] > maxVal)
            maxVal = data[i];
    }

    //free the allocated buffer.
    if (format[0] == "uint8")
        delete[] datac;
    else if (format[0] == "uint16")
        delete[] datai;

    cout << "Data loaded successfully" << endl;

    return data;
}

#endif