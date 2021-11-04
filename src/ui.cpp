#include "include/common/ui.h"

ui::UI::UI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL3_Init();
}

ui::UI::~UI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();
}

void ui::UI::newFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGLUT_NewFrame();
}

void ui::UI::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ui::UI::widget()
{
    ImGui::Begin("E0 271 - GAV");

    // // Text input for file location
    // static char file_location[128] = "";
    // ImGui::InputText("File Location", file_location, IM_ARRAYSIZE(file_location));

    // input text
    static char input_text[128] = "";
    ImGui::InputText("Iso Value", input_text, IM_ARRAYSIZE(input_text));

    static float slider_value = volumeRender->getIsoValue();

    // button
    if (ImGui::Button("Button"))
    {
        slider_value = atof(input_text);
    }

    // slider
    ImGui::SliderFloat("Slider", &slider_value, volumeRender->getMinVal(), volumeRender->getMaxVal());

    // change in slider
    if (slider_value != volumeRender->getIsoValue())
    {
        volumeRender->setIsoValue(slider_value);
        volumeRender->updateVBO();
    }

    ImGui::End();
}

void ui::UI::setVolumeRender(VolumeRender *volumeRender)
{
    this->volumeRender = volumeRender;
}