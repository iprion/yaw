#include "TestMyGLFWWindow.h"
#include <iostream>

TestMyGLFWWindow::TestMyGLFWWindow(const std::string& name, unsigned int w, unsigned int h) :
    ImGuiGLFWWindow(name,w,h) {

}

bool TestMyGLFWWindow::init() {
    triangle.init();
    return true;
}

void TestMyGLFWWindow::drawGL() {
    triangle.setUniform("color", 1.0f,1.0f,1.0f);
    triangle.draw();
}