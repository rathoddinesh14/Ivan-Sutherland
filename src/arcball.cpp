#include "include/common/arcball.h"

ArcBall::ArcBall(int width, int height, GLfloat speed):
    width(width), height(height), angle(0.0f), speed(speed)
{
    std::cout << "ArcBall::ArcBall() initialized" << std::endl;
    session = false;
    tracking = false;
}

ArcBall::~ArcBall() 
{
    
}

void ArcBall::handleMouseButton(int button, int state, int x, int y) 
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
        session = true;
    }
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
        session = false;
        tracking = false;
    }
}

void ArcBall::handleCursor(int x, int y)
{
    if (!session) return;
    if (!tracking)
    {
        tracking = true;
        last_mouse_pos = getNormalizedScreenCoord(x, y);
        return;
    }

    Vector3f curr_mouse_pos = getNormalizedScreenCoord(x, y);

    angle = acos( std::min(1.0f, last_mouse_pos.Dot(curr_mouse_pos)));

    camAxis = last_mouse_pos.Cross(curr_mouse_pos);
    camAxis.Normalize();
}

Vector3f ArcBall::getNormalizedScreenCoord(int x, int y)
{
    float xn = (2.0f * x) / (float)width - 1.0f;
    float yn = 1.0f - (2.0f * y) / (float)height;
    float zn = 0.0f;
    float len2 = xn * xn + yn * yn;
    if (len2 <= 1.0f)
    {
        zn = sqrt(1.0f - len2);
    }
    else
    {
        xn /= sqrt(len2);
        yn /= sqrt(len2);
    }

    Vector3f result(xn, yn, zn);
    result.Normalize();

    return result;
}

void ArcBall::updateWindowSize(int width, int height)
{
    this->width = width;
    this->height = height;
}

Matrix4f ArcBall::getRotationMatrix()
{
    Matrix4f rotation;
    rotation.InitIdentity();
    rotation.InitAxisRotateTransform(camAxis, -angle * speed);
    return rotation;
}