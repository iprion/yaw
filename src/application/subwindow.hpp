#pragma once

#include "vulkan/framebuffer.hpp"
#include "vulkan/device.hpp"

class SubWindow {

public :
    SubWindow(Device& device, const std::string& name, unsigned int w, unsigned int h);
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
  Device& device;
};
