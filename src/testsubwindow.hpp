#pragma once

#include "application/subwindow.hpp"
#include "vulkan/triangle.hpp"

class TestSubWindow : public SubWindow {

public:
    TestSubWindow(Device& device, const std::string& name, unsigned int w, unsigned int h);

protected :
    virtual void drawGL();
    virtual bool init();

private :
      Triangle triangle;
};
