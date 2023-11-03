#ifndef YAW_H
#define YAW_H

#include "application/application.h"
#include "application/subwindow.h"
#include "opengl/triangle.h"
#include "YawGLViewer.h" 

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

#endif
