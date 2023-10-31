#ifndef IMGUI_OPENGL_APP_H
#define IMGUI_OPENGL_APP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include <string>

class ImGuiGLFWApp {

public :
    bool build(const std::string& appName, unsigned int width, unsigned int height, bool dark);
    void start();
    void shutdown();
    unsigned int width() const { return width_; };
    unsigned int height() const { return height_; };

protected:
    virtual void ui() {};
    virtual void draw() {};
    virtual void update() {};
    virtual bool init() { return true; };

private : 
    static void glfwErrorCallback(int error, const char* description);
    void updateViewPort();
    void newFrame();
    void endFrame();
    void clear();
    void events();
    bool closed();

private :
    GLFWwindow* mainWindow;
    ImVec4 clear_color;    
    unsigned int height_;
    unsigned int width_;
    ImGuiIO* io;
};


#endif