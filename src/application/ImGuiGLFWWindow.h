#ifndef IMGUI_GLFW_WINDOW_H
#define IMGUI_GLFW_WINDOW_H

#include "opengl/framebuffer.h"


class ImGuiGLFWWindow {

public :
    ImGuiGLFWWindow(const std::string& name, unsigned int w, unsigned int h);
    bool ui();
    void draw();
    virtual void update() {};

protected:
    virtual void drawGL() {};
    virtual bool init() { return true; };

private :
  Framebuffer framebuffer;
  unsigned int width;
  unsigned int height;
  std::string windowName;
  bool isInitialized;
};


#endif