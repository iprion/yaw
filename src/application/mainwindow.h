#pragma once

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

#include "device.h"

class MainWindow {

public :
    MainWindow(const std::string& appName, unsigned int width, unsigned int height);
    ~MainWindow();
    
    GLFWwindow* window() { return mainWindow; };
    int width() const;
    int height() const;
    void updateViewPort();

    void clear();
    void events();
    bool closed();
    std::vector<const char*> requiredExtensions();
    VkSurfaceKHR createSurface(const Device& device);

protected:
    virtual void ui() {};
    virtual void draw() {};
    virtual void update() {};
    virtual bool init() { return true; };

private : 
    static void glfwErrorCallback(int error, const char* description);

private :
    GLFWwindow* mainWindow;
};
