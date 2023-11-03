#pragma once

#include "trackball/qglviewer.h"

class YawGLViewer : public QGLViewer {

public:
  virtual void draw();
  virtual void update();
  virtual void init();

};

