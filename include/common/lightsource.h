#ifndef __LIGHTSOURCE_H__
#define __LIGHTSOURCE_H__

#include "cube.h"

class LightSource: public Cube
{

    Vector3f position;

    public:
    LightSource();

    void render(const Matrix4f &VP);

    // setters
    void setPosition(Vector3f pos);

    // getters
    Vector3f getPosition();
};

#endif // __LIGHTSOURCE_H__