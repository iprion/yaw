#pragma once

#include "application/application.hpp"
#include "application/subwindow.hpp"
#include "vulkan/triangle.hpp"
#include "yawglviewer.hpp" 

class Yaw : public Application {

public:
  Yaw(const std::string& appName, unsigned int width, unsigned int height, bool dark);

protected:
    virtual void ui();
    virtual void draw();
    virtual void update();
    virtual bool init();

private :
  Triangle triangle;
  Triangle triangle2;

  //float rotation = 0.0;
  //float translation[2] = {0.0, 0.0};    
  //float color[4] = { 1.0f,1.0f,1.0f,1.0f };
  SubWindow * opengGLWindow1;
  SubWindow * opengGLWindow2;
  YawGLViewer viewer;

};
