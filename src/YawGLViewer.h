#ifndef YAW2_H
#define YAW2_H

#include "trackball/qglviewer.h"
#include "imgui.h"

class YawGLViewer : public QGLViewer {

public:
  virtual void draw();
  virtual void update();
  virtual void init();

};

#endif
