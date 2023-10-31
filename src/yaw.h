#ifndef YAW_H
#define YAW_H

#include "application/ImGuiGLFWApp.h"
#include "application/ImGuiGLFWWindow.h"
#include "opengl/triangle.h"
#include "YawGLViewer.h" 
class Yaw : public ImGuiGLFWApp {

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
  ImGuiGLFWWindow * opengGLWindow1;
  ImGuiGLFWWindow * opengGLWindow2;
  YawGLViewer viewer;

};

#endif
