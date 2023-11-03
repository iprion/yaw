#pragma once

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

#include "mainwindow.h"
#include "device.h"

#include <string>

class Application {

public :
    Application(const std::string& appName, unsigned int width, unsigned int height, bool dark);
    ~Application();

    void run();

protected:
    virtual void ui() {};
    virtual void draw() {};
    virtual void update() {};
    virtual bool init() { return true; };

private : 
    void SetupVulkanWindow(int width, int height);
    void CleanupVulkanWindow();
    void FrameRender(ImDrawData* draw_data);
    void FramePresent();

    void updateViewPort();
    void newFrame();
    void endFrame();
    void clear();
    void events();

private :
    MainWindow mainWindow;
    Device device;
    ImGui_ImplVulkanH_Window vulkanWindow;

    ImVec4 clear_color;    
    ImGuiIO* io;

};


