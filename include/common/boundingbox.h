#ifndef __BOUNDINGBOX_H__
#define __BOUNDINGBOX_H__

#include "cube.h"

class BoundingBox: public Cube
{

    public:
    BoundingBox();

    void render(const Matrix4f &VP);
};

#endif // __BOUNDINGBOX_H__