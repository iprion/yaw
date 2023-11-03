#include "testsubwindow.hpp"
#include <iostream>

TestSubWindow::TestSubWindow(Device& device,const std::string& name, unsigned int w, unsigned int h) :
    SubWindow(device, name,w,h),
    triangle(device) {

}

bool TestSubWindow::init() {
    triangle.init();
    return true;
}

void TestSubWindow::drawGL() {
    triangle.setUniform("color", 1.0f,1.0f,1.0f);
    triangle.draw();
}