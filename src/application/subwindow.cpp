#include "subwindow.h"
#include "imgui.h"
#include <iostream>

SubWindow::SubWindow(const std::string& name, unsigned int w, unsigned int h) {
    framebuffer.create(w, h);
    width = w; 
    height = h;
    windowName = name;
    isInitialized = false;
}

bool SubWindow::ui() {

    if (!isInitialized) {
        if (!init())
            return false;
        else
            isInitialized = true;
    }

    ImGui::SetNextWindowSize(ImVec2(width,height), ImGuiCond_FirstUseEver);

    ImGui::Begin(windowName.c_str());

    ImVec4 colorMultiplier(1.0f, 1.0f, 1.0f, 1.0f); 
    if (ImGui::IsWindowFocused() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        colorMultiplier.w = 0.1f;

    width = ImGui::GetContentRegionAvail().x;
    height = ImGui::GetContentRegionAvail().y;

    // we rescale the framebuffer to the actual window size here and reset the glViewport 
    framebuffer.resize(width, height);

    ImGui::Image(framebuffer.texture(), 
        ImVec2(width, height), 
        ImVec2(0, 1), 
        ImVec2(1, 0),colorMultiplier);

    ImGui::End();

    return true;
}

void SubWindow::draw() {

    // now we can bind our framebuffer
    framebuffer.bind();
    
    glViewport(0, 0, width, height);

    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);  

    // and we render our triangle as before
    drawGL();

    // and unbind it again 
    framebuffer.unbind();
}