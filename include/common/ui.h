#ifndef __UI_H__
#define __UI_H__

#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl3.h"
#include "volumerender.h"


#include<stdio.h>

namespace ui
{

    class UI
    {
    private:
        VolumeRender *volumeRender;
    public:
        UI();
        void newFrame();
        void render();
        void widget();
        void setVolumeRender(VolumeRender *volumeRender);
        ~UI();
    };
};

#endif // __UI_H__