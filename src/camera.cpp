#include "include/common/camera.h"

static int MARGIN = 10;
static float EDGE_STEP = 1.0f;

Camera::Camera(int w, int h) : pos(0.0f, 0.0f, 4.0f),
                               tar(0.0f, 0.0f, 1.0f),
                               Up(0.0f, 1.0f, 0.0f), speed(0.5f), width(w), height(h)
{
    Vector3f HTarget(tar.x, 0.0, tar.z);
    HTarget.Normalize();

    float Angle = ToDegree(asin(abs(HTarget.z)));

    if (HTarget.z >= 0.0f)
    {
        if (HTarget.x >= 0.0f)
        {
            angleH = 360.0f - Angle;
        }
        else
        {
            angleH = 180.0f + Angle;
        }
    }
    else
    {
        if (HTarget.x >= 0.0f)
        {
            angleH = Angle;
        }
        else
        {
            angleH = 180.0f - Angle;
        }
    }

    angleV = -ToDegree(asin(tar.y));

    onTopEdge = false;
    onBottomEdge = false;
    onLeftEdge = false;
    onRightEdge = false;
    mousePos.x = width / 2;
    mousePos.y = height / 2;

    printf("Camera created\n");
}

void Camera::rotate(const Vector3f &axis, float angle)
{
    Matrix4f rot;
    rot.InitAxisRotateTransform(axis, angle);
    Vector4f newUp = rot * Vector4f(Up.x, Up.y, Up.z, 1.0f);
    Up = Vector3f(newUp.x, newUp.y, newUp.z).Normalize();
}

Ray Camera::generateRay(int X, int Y)
{
    Vector3f dir = tar +
                   (Up.Cross(tar).Normalize()) * (2 * (X + 0.5f) / width - 1) + Up * (2 * (Y + 0.5f) / height - 1) - pos;
    return Ray(pos, dir);
}

Matrix4f Camera::getMatrix()
{
    // camera transformation
    Matrix4f ct, transMat;
    transMat.InitTranslationTransform(-pos.x, -pos.y, -pos.z);
    ct.InitCameraTransform(tar, Up);
    return ct * transMat;
}

void Camera::updatePos(float x, float y, float z)
{
    pos.x += x;
    pos.y += y;
    pos.z += z;
}

void Camera::updateTar(float x, float y, float z)
{
    tar.x += x;
    tar.y += y;
    tar.z += z;
}

void Camera::print()
{
    std::cout << "Camera: " << std::endl;
    std::cout << "pos: " << pos.x << " " << pos.y << " " << pos.z << std::endl;
    std::cout << "tar: " << tar.x << " " << tar.y << " " << tar.z << std::endl;
    std::cout << "Up: " << Up.x << " " << Up.y << " " << Up.z << std::endl;
}

Vector3f Camera::getPos() const
{
    return pos;
}

void Camera::handleKeyboard(unsigned char key)
{
    switch (key)
    {
    case 'w':
        updatePos(speed * tar.x, speed * tar.y, speed * tar.z);
        break;
    case 's':
        updatePos(-speed * tar.x, -speed * tar.y, -speed * tar.z);
        break;
    case 'a':
    {
        Vector3f left = tar.Cross(Up).Normalize();
        updatePos(speed * left.x, speed * left.y, speed * left.z);
        break;
    }
    case 'd':
    {
        Vector3f right = Up.Cross(tar).Normalize();
        updatePos(speed * right.x, speed * right.y, speed * right.z);
        break;
    }
    case GLUT_KEY_PAGE_UP:
        updatePos(0.0f, speed, 0.0f);
        break;
    case GLUT_KEY_PAGE_DOWN:
        updatePos(0.0f, -speed, 0.0f);
        break;
    case '+':
        speed += 0.1f;
        // print speed
        std::cout << "speed: " << speed << std::endl;
        break;
    case '-':
        speed -= 0.1f;
        // check if speed is negative
        if (speed < 0.1f)
            speed = 0.1f;
        std::cout << "speed: " << speed << std::endl;
        break;
    default:
        break;
    }
}

void Camera::onRender()
{
    bool ShouldUpdate = false;

    if (onLeftEdge)
    {
        angleH -= EDGE_STEP;
        ShouldUpdate = true;
    }
    else if (onRightEdge)
    {
        angleH += EDGE_STEP;
        ShouldUpdate = true;
    }

    if (onTopEdge)
    {
        if (angleV > -90.0f)
        {
            angleV -= EDGE_STEP;
            ShouldUpdate = true;
        }
    }
    else if (onBottomEdge)
    {
        if (angleV < 90.0f)
        {
            angleV += EDGE_STEP;
            ShouldUpdate = true;
        }
    }

    if (ShouldUpdate)
    {
        update();
    }
}

void Camera::handleMouse(int x, int y)
{
    int DeltaX = x - mousePos.x;
    int DeltaY = y - mousePos.y;

    mousePos.x = x;
    mousePos.y = y;

    angleH += (float)DeltaX / 20.0f;
    angleV += (float)DeltaY / 50.0f;

    if (DeltaX == 0)
    {
        if (x <= MARGIN)
        {
            onLeftEdge = true;
        }
        else if (x >= (width - MARGIN))
        {
            onRightEdge = true;
        }
    }
    else
    {
        onLeftEdge = false;
        onRightEdge = false;
    }

    if (DeltaY == 0)
    {
        if (y <= MARGIN)
        {
            onTopEdge = true;
        }
        else if (y >= (height - MARGIN))
        {
            onBottomEdge = true;
        }
    }
    else
    {
        onTopEdge = false;
        onBottomEdge = false;
    }

    update();
}

void Camera::update()
{
    Vector3f Yaxis(0.0f, 1.0f, 0.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    Vector3f View(1.0f, 0.0f, 0.0f);
    View.Rotate(angleH, Yaxis);
    View.Normalize();

    // Rotate the view vector by the vertical angle around the horizontal axis
    Vector3f U = Yaxis.Cross(View);
    U.Normalize();
    View.Rotate(angleV, U);

    tar = View;
    tar.Normalize();

    Up = tar.Cross(U);
    Up.Normalize();
}

void Camera::Animate(float dt)
{
    Vector3f d = pos - tar;
    pos = Vector3f(d.x * cos(dt) + d.z * sin(dt), d.y, -d.x * sin(dt) + d.z * cos(dt)) + tar;
}