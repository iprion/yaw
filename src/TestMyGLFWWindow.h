#ifndef TEST_MY_GLFW_WINDOW_H
#define TEST_MY_GLFW_WINDOW_H

#include "application/ImGuiGLFWWindow.h"
#include "opengl/triangle.h"

class TestMyGLFWWindow : public ImGuiGLFWWindow {

public:
    TestMyGLFWWindow(const std::string& name, unsigned int w, unsigned int h);

protected :
    virtual void drawGL();
    virtual bool init();

private :
      Triangle triangle;
};


#endif