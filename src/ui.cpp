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
        if (volumeRender->getAlgo() != 2)
            volumeRender->updateVBO();
    }

    // Anti aliasing with 0, 2, 4, 8, 16 samples
    static int aa_samples = 0;
    static const char *aa_items[] = {"0", "2", "4", "8", "16"};
    ImGui::Combo("Anti Aliasing", &aa_samples, aa_items, IM_ARRAYSIZE(aa_items));

    // change in anti aliasing
    if (aa_samples == 0)
    {
        // disable anti aliasing
        glDisable(GL_MULTISAMPLE);
    }
    else if (aa_samples == 1)
    {
        // enable 2x anti aliasing
        glEnable(GL_MULTISAMPLE);
        // set anti aliasing to 2x
        glSampleCoverage(2.0f, GL_TRUE);
    }
    else if (aa_samples == 2)
    {
        // enable 4x anti aliasing
        glEnable(GL_MULTISAMPLE);
        // set anti aliasing to 4x
        glSampleCoverage(4.0f, GL_TRUE);
    }
    else if (aa_samples == 3)
    {
        // enable 8x anti aliasing
        glEnable(GL_MULTISAMPLE);
        // set anti aliasing to 8x
        glSampleCoverage(8.0f, GL_TRUE);
    }
    else if (aa_samples == 4)
    {
        // enable 16x anti aliasing
        glEnable(GL_MULTISAMPLE);
        // set anti aliasing to 16x
        glSampleCoverage(16.0f, GL_TRUE);
    }

    // drop down menu
    static int selected_item = 0;
    static const char *items[] = {"Naive technique", "Domain search technique", "GPU - Ray marching"};
    ImGui::Combo("Algorithm", &selected_item, items, IM_ARRAYSIZE(items));

    if (selected_item == 0)
    {
        volumeRender->setAlgo(0);
    }
    else if (selected_item == 1)
    {
        volumeRender->setAlgo(1);
    }
    else if (selected_item == 2)
    {
        volumeRender->setAlgo(2);
    }

    ImGui::End();
}

void ui::UI::setVolumeRender(VolumeRender *volumeRender)
{
    this->volumeRender = volumeRender;
}