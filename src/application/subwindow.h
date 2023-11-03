#pragma once

#include "opengl/framebuffer.h"

class SubWindow {

public :
    SubWindow(const std::string& name, unsigned int w, unsigned int h);
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
