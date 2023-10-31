#ifndef QGLVIEWER_QGLVIEWER_H
#define QGLVIEWER_QGLVIEWER_H

#include "camera.h"
#include <GLFW/glfw3.h>
#include "Signaler.h"

namespace qglviewer {
class MouseGrabber;
class ManipulatedFrame;
class ManipulatedCameraFrame;
} // namespace qglviewer

/*! \brief A versatile 3D OpenGL viewer based on QOpenGLWidget.
\class QGLViewer qglviewer.h QGLViewer/qglviewer.h

It features many classical viewer functionalities, such as a camera trackball,
manipulated objects, snapshot saving and much <a
href="../features.html">more</a>. Its main goal is to ease the development of
new 3D applications.

New users should read the <a href="../introduction.html">introduction page</a>
to get familiar with important notions such as sceneRadius(), sceneCenter() and
the world coordinate system. Try the numerous simple <a
href="../examples/index.html">examples</a> to discover the possibilities and
understand how it works.

<h3>Usage</h3>

To use a QGLViewer, derive you viewer class from the QGLViewer and overload its
draw() virtual method. See the <a
href="../examples/simpleViewer.html">simpleViewer example</a> for an
illustration.

An other option is to connect your drawing methods to the signals emitted by the
QGLViewer (Qt's callback mechanism). See the <a
href="../examples/callback.html">callback example</a> for a complete
implementation.

\nosubgrouping */
class QGLViewer : public Signaler {

public:
  explicit QGLViewer();
  virtual ~QGLViewer();

  /*! @name Display of visual hints */
  //@{
public:
  /*! Returns \c true if the world axis is drawn by the viewer.

  Set by setAxisIsDrawn() or toggleAxisIsDrawn(). Default value is \c false. */
  bool axisIsDrawn() const { return axisIsDrawn_; }
  /*! Returns \c true if a XY grid is drawn by the viewer.

  Set by setGridIsDrawn() or toggleGridIsDrawn(). Default value is \c false. */
  bool gridIsDrawn() const { return gridIsDrawn_; }
  /*! Returns \c true if the viewer displays the current frame rate (Frames Per
  Second).

  Use QApplication::setFont() to define the display font (see drawText()).

  Set by setFPSIsDisplayed() or toggleFPSIsDisplayed(). Use currentFPS() to get
  the current FPS. Default value is \c false. */
  bool FPSIsDisplayed() const { return FPSIsDisplayed_; }
  /*! Returns \c true if text display (see drawText()) is enabled.

  Set by setTextIsEnabled() or toggleTextIsEnabled(). This feature conveniently
  removes all the possibly displayed text, cleaning display. Default value is \c
  true. */
  bool textIsEnabled() const { return textIsEnabled_; }

  /*! Returns \c true if the camera() is being edited in the viewer.

  Set by setCameraIsEdited() or toggleCameraIsEdited(). Default value is \p
  false.

  The current implementation is limited: the defined camera() paths (see
  qglviewer::Camera::keyFrameInterpolator()) are simply displayed using
  qglviewer::Camera::drawAllPaths(). Actual camera and path edition will be
  implemented in the future. */
  bool cameraIsEdited() const { return cameraIsEdited_; }

public:
  /*! Sets the state of axisIsDrawn(). Emits the axisIsDrawnChanged() signal.
   * See also toggleAxisIsDrawn(). */
  void setAxisIsDrawn(bool draw = true) {
    axisIsDrawn_ = draw;
    emit<bool>("axisIsDrawnChanged", draw);
    update();
  }
  /*! Sets the state of gridIsDrawn(). Emits the gridIsDrawnChanged() signal.
   * See also toggleGridIsDrawn(). */
  void setGridIsDrawn(bool draw = true) {
    gridIsDrawn_ = draw;
    emit<bool>("gridIsDrawnChanged", draw);
    update();
  }
  /*! Sets the state of FPSIsDisplayed(). Emits the FPSIsDisplayedChanged()
   * signal. See also toggleFPSIsDisplayed(). */
  void setFPSIsDisplayed(bool display = true) {
    FPSIsDisplayed_ = display;
    emit<bool>("FPSIsDisplayedChanged", display);
    update();
  }
  /*! Sets the state of textIsEnabled(). Emits the textIsEnabledChanged()
   * signal. See also toggleTextIsEnabled(). */
  void setTextIsEnabled(bool enable = true) {
    textIsEnabled_ = enable;
    emit<bool>("textIsEnabledChanged",enable);
    update();
  }
  void setCameraIsEdited(bool edit = true);

  /*! Toggles the state of axisIsDrawn(). See also setAxisIsDrawn(). */
  void toggleAxisIsDrawn() { setAxisIsDrawn(!axisIsDrawn()); }
  /*! Toggles the state of gridIsDrawn(). See also setGridIsDrawn(). */
  void toggleGridIsDrawn() { setGridIsDrawn(!gridIsDrawn()); }
  /*! Toggles the state of FPSIsDisplayed(). See also setFPSIsDisplayed(). */
  void toggleFPSIsDisplayed() { setFPSIsDisplayed(!FPSIsDisplayed()); }
  /*! Toggles the state of textIsEnabled(). See also setTextIsEnabled(). */
  void toggleTextIsEnabled() { setTextIsEnabled(!textIsEnabled()); }
  /*! Toggles the state of cameraIsEdited(). See also setCameraIsEdited(). */
  void toggleCameraIsEdited() { setCameraIsEdited(!cameraIsEdited()); }
  //@}

  /*! @name Viewer's colors */
  //@{
public:
  /*! Returns the background color of the viewer.

  This method is provided for convenience since the background color is an
  OpenGL state variable set with \c glClearColor(). However, this internal
  representation has the advantage that it is saved (resp. restored) with
  saveStateToFile() (resp. restoreStateFromFile()).

  Use setBackgroundColor() to define and activate a background color.

  \attention Each QColor component is an integer ranging from 0 to 255. This
  differs from the qreal values used by \c glClearColor() which are in the
  0.0-1.0 range. Default value is (51, 51, 51) (dark gray). You may have to
  change foregroundColor() accordingly.

  \attention This method does not return the current OpenGL clear color as \c
  glGet() does. Instead, it returns the QGLViewer internal variable. If you
  directly use \c glClearColor() or \c qglClearColor() instead of
  setBackgroundColor(), the two results will differ. */
  QColor backgroundColor() const { return backgroundColor_; }

  /*! Returns the foreground color used by the viewer.

  This color is used when FPSIsDisplayed(), gridIsDrawn(), to display the camera
  paths when the cameraIsEdited().

  \attention Each QColor component is an integer in the range 0-255. This
  differs from the qreal values used by \c glColor3f() which are in the range
  0-1. Default value is (180, 180, 180) (light gray).

  Use \c qglColor(foregroundColor()) to set the current OpenGL color to the
  foregroundColor().

  See also backgroundColor(). */
  QColor foregroundColor() const { return foregroundColor_; }
public:
  /*! Sets the backgroundColor() of the viewer and calls \c qglClearColor(). See
     also setForegroundColor(). */
  void setBackgroundColor(const QColor &color) {
    backgroundColor_ = color;
    glClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
  }
  /*! Sets the foregroundColor() of the viewer, used to draw visual hints. See
   * also setBackgroundColor(). */
  void setForegroundColor(const QColor &color) { foregroundColor_ = color; }
  //@}

  /*! @name Scene dimensions */
  //@{
public:
  /*! Returns the scene radius.

  The entire displayed scene should be included in a sphere of radius
  sceneRadius(), centered on sceneCenter().

  This approximate value is used by the camera() to set
  qglviewer::Camera::zNear() and qglviewer::Camera::zFar(). It is also used to
  showEntireScene() or to scale the world axis display..

  Default value is 1.0. This method is equivalent to camera()->sceneRadius().
  See setSceneRadius(). */
  qreal sceneRadius() const { return camera()->sceneRadius(); }
  /*! Returns the scene center, defined in world coordinates.

  See sceneRadius() for details.

  Default value is (0,0,0). Simply a wrapper for camera()->sceneCenter(). Set
  using setSceneCenter().

  Do not mismatch this value (that only depends on the scene) with the
  qglviewer::Camera::pivotPoint(). */
  qglviewer::Vec sceneCenter() const { return camera()->sceneCenter(); }

public:
  /*! Sets the sceneRadius().

    The camera() qglviewer::Camera::flySpeed() is set to 1% of this value by
    this method. Simple wrapper around camera()->setSceneRadius(). */
  virtual void setSceneRadius(qreal radius) {
    camera()->setSceneRadius(radius);
  }

  /*! Sets the sceneCenter(), defined in world coordinates.

    \attention The qglviewer::Camera::pivotPoint() is set to the sceneCenter()
    value by this method. */
  virtual void setSceneCenter(const qglviewer::Vec &center) {
    camera()->setSceneCenter(center);
  }

  /*! Convenient way to call setSceneCenter() and setSceneRadius() from a (world
    axis aligned) bounding box of the scene.

    This is equivalent to:
    \code
    setSceneCenter((min+max) / 2.0);
    setSceneRadius((max-min).norm() / 2.0);
    \endcode */
  void setSceneBoundingBox(const qglviewer::Vec &min,
                           const qglviewer::Vec &max) {
    camera()->setSceneBoundingBox(min, max);
  }

  /*! Moves the camera so that the entire scene is visible.

    Simple wrapper around qglviewer::Camera::showEntireScene(). */
  void showEntireScene() {
    camera()->showEntireScene();
    update();
  }
  //@}

  /*! @name Associated objects */
  //@{
public:
  /*! Returns the associated qglviewer::Camera, never \c nullptr. */
  qglviewer::Camera *camera() const { return camera_; }

  /*! Returns the viewer's qglviewer::ManipulatedFrame.

  This qglviewer::ManipulatedFrame can be moved with the mouse when the
  associated mouse bindings are used (default is when pressing the \c Control
  key with any mouse button). Use setMouseBinding() to define new bindings.

  See the <a href="../examples/manipulatedFrame.html">manipulatedFrame
  example</a> for a complete implementation.

  Default value is \c nullptr, meaning that no qglviewer::ManipulatedFrame is set.
*/
  qglviewer::ManipulatedFrame *manipulatedFrame() const {
    return manipulatedFrame_;
  }

public:
  void setCamera(qglviewer::Camera *const camera);
  void setManipulatedFrame(qglviewer::ManipulatedFrame *frame);
  //@}

  /*! @name Mouse grabbers */
  //@{
public:
  /*! Returns the current qglviewer::MouseGrabber, or \c nullptr if no
  qglviewer::MouseGrabber currently grabs mouse events.

  When qglviewer::MouseGrabber::grabsMouse(), the different mouse events are
  sent to the mouseGrabber() instead of their usual targets (camera() or
  manipulatedFrame()).

  See the qglviewer::MouseGrabber documentation for details on MouseGrabber's
  mode of operation.

  In order to use MouseGrabbers, you need to enable mouse tracking (so that
  mouseMoveEvent() is called even when no mouse button is pressed). Add this
  line in init() or in your viewer constructor: \code setMouseTracking(true);
  \endcode
  Note that mouse tracking is disabled by default. Use
  QWidget::hasMouseTracking() to retrieve current state. */
  qglviewer::MouseGrabber *mouseGrabber() const { return mouseGrabber_; }

  void
  setMouseGrabberIsEnabled(const qglviewer::MouseGrabber *const mouseGrabber,
                           bool enabled = true);
  /*! Returns \c true if \p mouseGrabber is enabled.

  Default value is \c true for all MouseGrabbers. When set to \c false using
  setMouseGrabberIsEnabled(), the specified \p mouseGrabber will never become
  the mouseGrabber() of this QGLViewer. This is useful when you use several
  viewers: some MouseGrabbers may only have a meaning for some specific viewers
  and should not be selectable in others.

  You can also use qglviewer::MouseGrabber::removeFromMouseGrabberPool() to
  completely disable a MouseGrabber in all the QGLViewers. */
  bool
  mouseGrabberIsEnabled(const qglviewer::MouseGrabber *const mouseGrabber) {
    return disabledMouseGrabbers_.find(reinterpret_cast<size_t>(mouseGrabber)) == disabledMouseGrabbers_.end();
  }

public:
  void setMouseGrabber(qglviewer::MouseGrabber *mouseGrabber);
  //@}

  /*! @name State of the viewer */
  //@{
public:
  /*! Returns the aspect ratio of the viewer's widget (width() / height()). */
  qreal aspectRatio() const { return width() / static_cast<qreal>(height()); }
  /*! Returns the current averaged viewer frame rate.

  This value is computed and averaged over 20 successive frames. It only changes
  every 20 draw() (previously computed value is otherwise returned).

  This method is useful for true real-time applications that may adapt their
  computational load accordingly in order to maintain a given frequency.

  This value is meaningful only when draw() is regularly called, either using a
  \c QTimer, when animationIsStarted() or when the camera is manipulated with
  the mouse.  */
  qreal currentFPS() { return f_p_s_; }
  /*! Returns \c true if the viewer is in fullScreen mode.

  Default value is \c false. Set by setFullScreen() or toggleFullScreen().

  Note that if the QGLViewer is embedded in an other QWidget, it returns \c true
  when the top level widget is in full screen mode. */
  bool isFullScreen() const { return fullScreen_; }

  /*! Returns the recommended size for the QGLViewer. Default value is 600x400
   * pixels. */
  virtual QSize sizeHint() const { return QSize(600, 400); }

public:
  void setFullScreen(bool fullScreen = true);
  /*! Toggles the state of isFullScreen(). See also setFullScreen(). */
  void toggleFullScreen() { setFullScreen(!isFullScreen()); }
  void toggleCameraMode();

private:
  bool cameraIsInRotateMode() const;
  //@}

  /*! @name Display methods */
  //@{
public:
  static void drawArrow(qreal length = 1.0, qreal radius = -1.0,
                        int nbSubdivisions = 12);
  static void drawArrow(const qglviewer::Vec &from, const qglviewer::Vec &to,
                        qreal radius = -1.0, int nbSubdivisions = 12);
  static void drawAxis(qreal length = 1.0);
  static void drawGrid(qreal size = 1.0, int nbSubdivisions = 10);

  virtual void startScreenCoordinatesSystem(bool upward = false) const;
  virtual void stopScreenCoordinatesSystem() const;

  void drawText(int x, int y, const std::string &text, const QFont &fnt = QFont());
  void displayMessage(const std::string &message, int delay = 2000);
  // void draw3DText(const qglviewer::Vec& pos, const qglviewer::Vec& normal,
  // const std::string& string, GLfloat height=0.1);

protected:
  virtual void drawLight(GLenum light, qreal scale = 1.0) const;

private:
  void displayFPS();
  /*! Vectorial rendering callback method. */
  void drawVectorial() { paintGL(); }

  friend void drawVectorial(void *param);

//@}

  /*! @name Useful inherited methods */
  //@{
public:
  /*! Returns viewer's widget width (in pixels). See QOpenGLWidget
   * documentation. */
  int width() const { return 100; };
  /*! Returns viewer's widget height (in pixels). See QOpenGLWidget
   * documentation. */
  int height() const { return 100;}
  /*! Updates the display. Do not call draw() directly, use this method instead.
   * See QOpenGLWidget documentation. */
  virtual void update() {}
  /*! Returns \c true if the widget has a valid GL rendering context. See
  QOpenGLWidget documentation. */
  /*! Makes this widget's rendering context the current OpenGL rendering
  context. Useful with several viewers. See QOpenGLWidget documentation. */
  /*! Returns \c true if mouseMoveEvent() is called even when no mouse button is
  pressed.

  You need to setMouseTracking() to \c true in order to use MouseGrabber (see
  mouseGrabber()). See details in the QWidget documentation. */
  bool hasMouseTracking() const { return true; } //??


  /* This class is used internally for screenshot that require tiling (image
  size size different from window size). Only in that case, is the private
  tileRegion_ pointer non null. It then contains the current tiled region, which
  is used by startScreenCoordinatesSystem to adapt the coordinate system. Not
  using it would result in a tiled drawing of the parts that use
  startScreenCoordinatesSystem. Also used by scaledFont for same purposes. */
  class TileRegion {
  public:
    qreal xMin, yMin, xMax, yMax, textScale;
  };

public:
  /*! Return a possibly scaled version of \p font, used for snapshot rendering.

  From a user's point of view, this method simply returns \p font and can be
  used transparently.

  However when internally rendering a screen snapshot using saveSnapshot(), it
  returns a scaled version of the font, so that the size of the rendered text on
  the snapshot is identical to what is displayed on screen, even if the snapshot
  uses image tiling to create an image of dimensions different from those of the
  current window. This scaled version will only be used when saveSnapshot()
  calls your draw() method to generate the snapshot.

  All your calls to renderText() function hence should use this method.
  \code
  renderText(x, y, z, "My Text", scaledFont(QFont()));
  \endcode
  will guarantee that this text will be properly displayed on arbitrary sized
  snapshots.

  Note that this method is not needed if you use drawText() which already calls
  it internally. */
  QFont scaledFont(const QFont &font) const {
    if (tileRegion_ == nullptr)
      return font;
    else {
      QFont f(font);
      if (f.pixelSize() == -1)
        f.setPointSizeF(f.pointSizeF() * tileRegion_->textScale);
      else
        f.setPixelSize(int(f.pixelSize() * tileRegion_->textScale));
      return f;
    }
  }
  //@}

  /*! @name Buffer to texture */
  //@{
public:
  GLuint bufferTextureId() const;
  /*! Returns the texture coordinate corresponding to the u extremum of the
  bufferTexture.

  The bufferTexture is created by copyBufferToTexture(). The texture size has
  powers of two dimensions and the buffer image hence only fills a part of it.
  This value corresponds to the u coordinate of the extremum right side of the
  buffer image.

  Use (0,0) to (bufferTextureMaxU(), bufferTextureMaxV()) texture coordinates to
  map the entire texture on a quad. */
  qreal bufferTextureMaxU() const { return bufferTextureMaxU_; }
  /*! Same as bufferTextureMaxU(), but for the v texture coordinate. */
  qreal bufferTextureMaxV() const { return bufferTextureMaxV_; }
  // These methods are part of the QGLWidget public API.
  // As of version 2.7.0, the use of QOpenGLWidget instead means that they have
  // to be provided for backward compatibility.
  void renderText(int x, int y, const std::string &str,
                  const QFont &font = QFont());
  void renderText(double x, double y, double z, const std::string &str,
                  const QFont &font = QFont());

public:
  void copyBufferToTexture(GLint internalFormat, GLenum format = GL_NONE);
  //@}

  /*! @name Animation */
  //@{
public:
  /*! Return \c true when the animation loop is started.

  During animation, an infinite loop calls animate() and draw() and then waits
  for animationPeriod() milliseconds before calling animate() and draw() again.
  And again.

  Use startAnimation(), stopAnimation() or toggleAnimation() to change this
  value.

  See the <a href="../examples/animation.html">animation example</a> for
  illustration. */
  bool animationIsStarted() const { return animationStarted_; }
  /*! The animation loop period, in milliseconds.

  When animationIsStarted(), this is delay waited after draw() to call animate()
  and draw() again. Default value is 40 milliseconds (25 Hz).

  This value will define the currentFPS() when animationIsStarted() (provided
  that your animate() and draw() methods are fast enough).

  If you want to know the maximum possible frame rate of your machine on a given
  scene, setAnimationPeriod() to \c 0, and startAnimation() (keyboard shortcut
  is \c Enter). The display will then be updated as often as possible, and the
  frame rate will be meaningful.

  \note This value is taken into account only the next time you call
  startAnimation(). If animationIsStarted(), you should stopAnimation() first.
*/
  int animationPeriod() const { return animationPeriod_; }

public:
  /*! Sets the animationPeriod(), in milliseconds. */
  void setAnimationPeriod(int period) { animationPeriod_ = period; }
  virtual void startAnimation();
  virtual void stopAnimation();
  /*! Scene animation method.

    When animationIsStarted(), this method is in charge of the scene update
    before each draw(). Overload it to define how your scene evolves over time.
    The time should either be regularly incremented in this method (frame-rate
    independent animation) or computed from actual time (for instance using
    QTime::elapsed()) for real-time animations.

        Note that KeyFrameInterpolator (which regularly updates a Frame) does
    not use this method to animate a Frame, but rather rely on a QTimer
    signal-slot mechanism.

    See the <a href="../examples/animation.html">animation example</a> for an
    illustration. */
  virtual void animate() { 
    emit("animateNeeded"); 
  }
  /*! Calls startAnimation() or stopAnimation(), depending on
   * animationIsStarted(). */
  void toggleAnimation() {
    if (animationIsStarted())
      stopAnimation();
    else
      startAnimation();
  }
  //@}

public:

  /*! Signal emitted by the default init() method.

  Connect this signal to the methods that need to be called to initialize your
  viewer or overload init(). */
  void viewerInitialized();

  /*! Signal emitted by the default draw() method.

  Connect this signal to your main drawing method or overload draw(). See the <a
  href="../examples/callback.html">callback example</a> for an illustration. */
  void drawNeeded();

  /*! Signal emitted at the end of the QGLViewer::paintGL() method, when frame
  is drawn.

  Can be used to notify an image grabbing process that the image is ready. A
  typical example is to connect this signal to the saveSnapshot() method, so
  that a (numbered) snapshot is generated after each new display, in order to
  create a movie: \code connect(viewer, SIGNAL(drawFinished(bool)),
  SLOT(saveSnapshot(bool))); \endcode

  The \p automatic bool variable is always \c true and has been added so that
  the signal can be connected to saveSnapshot() with an \c automatic value set
  to \c true. */
  void drawFinished(bool automatic);

  /*! Signal emitted by the default animate() method.

  Connect this signal to your scene animation method or overload animate(). */
  void animateNeeded();

  /*! Signal emitted by the default QGLViewer::help() method.

  Connect this signal to your own help method or overload help(). */
  void helpRequired();

  /*! This signal is emitted whenever axisIsDrawn() changes value. */
  void axisIsDrawnChanged(bool drawn);
  /*! This signal is emitted whenever gridIsDrawn() changes value. */
  void gridIsDrawnChanged(bool drawn);
  /*! This signal is emitted whenever FPSIsDisplayed() changes value. */
  void FPSIsDisplayedChanged(bool displayed);
  /*! This signal is emitted whenever textIsEnabled() changes value. */
  void textIsEnabledChanged(bool enabled);
  /*! This signal is emitted whenever cameraIsEdited() changes value.. */
  void cameraIsEditedChanged(bool edited);
  /*! Signal emitted by select().

  Connect this signal to your selection method or overload select(), or more
  probably simply drawWithNames(). */
  void pointSelected(const QMouseEvent *e);

  /*! Signal emitted by setMouseGrabber() when the mouseGrabber() is changed.

  \p mouseGrabber is a pointer to the new MouseGrabber. Note that this signal is
  emitted with a \c nullptr parameter each time a MouseGrabber stops grabbing
  mouse. */
  void mouseGrabberChanged(qglviewer::MouseGrabber *mouseGrabber);

  /*! @name Help window */
  //@{
public:
  /*! Returns the std::string displayed in the help() window main tab.

  Overload this method to define your own help string, which should shortly
  describe your application and explain how it works. Rich-text (HTML) tags can
  be used (see QStyleSheet() documentation for available tags): \code std::string
  myViewer::helpString() const
  {
  std::string text("<h2>M y V i e w e r</h2>");
  text += "Displays a <b>Scene</b> using OpenGL. Move the camera using the
  mouse."; return text;
  }
  \endcode

  See also mouseString() and keyboardString(). */
  virtual std::string helpString() const { return "No help available."; }

  virtual std::string mouseString() const;
  virtual std::string keyboardString() const;

#ifndef DOXYGEN
  /*! This method is deprecated, use mouseString() instead. */
  virtual std::string mouseBindingsString() const { return mouseString(); }
  /*! This method is deprecated, use keyboardString() instead. */
  virtual std::string shortcutBindingsString() const { return keyboardString(); }
#endif

public: 
  virtual void help();
  virtual void aboutQGLViewer();

protected:



public:
  virtual void resizeGL(int width, int height);
  virtual void initializeGL();
  virtual void paintGL();
  virtual void preDraw();
  virtual void shutDown() {};

  /*! Initializes the viewer OpenGL context.

  This method is called before the first drawing and should be overloaded to
  initialize some of the OpenGL flags. The default implementation is empty. See
  initializeGL().

  Typical usage include camera() initialization (showEntireScene()), previous
  viewer state restoration (restoreStateFromFile()), OpenGL state modification
  and display list creation.

  Note that initializeGL() modifies the standard OpenGL context. These values
  can be restored back in this method.

  \attention You should not call updateGL() (or any method that calls it) in
  this method, as it will result in an infinite loop. The different QGLViewer
  set methods (setAxisIsDrawn(), setFPSIsDisplayed()...) are protected against
  this problem and can safely be called.

  \note All the OpenGL specific initializations must be done in this method: the
  OpenGL context is not yet available in your viewer constructor. */
  virtual void init() { 
    emit("viewerInitialized"); 
  }


  /*! The core method of the viewer, that draws the scene.

  If you build a class that inherits from QGLViewer, this is the method you want
  to overload. See the <a href="../examples/simpleViewer.html">simpleViewer
  example</a> for an illustration.

  The camera modelView matrix set in preDraw() converts from the world to the
  camera coordinate systems. Vertices given in draw() can then be considered as
  being given in the world coordinate system. The camera is moved in this world
  using the mouse. This representation is much more intuitive than the default
  camera-centric OpenGL standard.

  \attention The \c GL_PROJECTION matrix should not be modified by this method,
  to correctly display visual hints (axis, grid, FPS...) in postDraw(). Use
  push/pop or call camera()->loadProjectionMatrix() at the end of draw() if you
  need to change the projection matrix (unlikely). On the other hand, the \c
  GL_MODELVIEW matrix can be modified and left in a arbitrary state. */
  virtual void draw() {}
  virtual void fastDraw();
  virtual void postDraw();
  //@}

  /*! @name Mouse, keyboard and event handlers */
  //@{
protected:
  virtual void mousePressEvent(QMouseEvent *);
  virtual void mouseMoveEvent(QMouseEvent *);
  virtual void mouseReleaseEvent(QMouseEvent *);
  virtual void mouseDoubleClickEvent(QMouseEvent *);
  virtual void wheelEvent(QWheelEvent *);
  virtual void keyPressEvent(QKeyEvent *);
  virtual void keyReleaseEvent(QKeyEvent *);
  virtual void timerEvent(QTimerEvent *);
  //@}

  /*! @name Object selection */
  //@{
public:
  /*! Returns the name (an integer value) of the entity that was last selected
  by select(). This value is set by endSelection(). See the select()
  documentation for details.

  As a convention, this method returns -1 if the selectBuffer() was empty,
  meaning that no object was selected.

  Return value is -1 before the first call to select(). This value is modified
  using setSelectedName(). */
  int selectedName() const { return selectedObjectId_; }
  /*! Returns the selectBuffer() size.

  See the select() documentation for details. Use setSelectBufferSize() to
  change this value.

  Default value is 4000 (i.e. 1000 objects in selection region, since each
  object pushes 4 values). This size should be over estimated to prevent a
  buffer overflow when many objects are drawn under the mouse cursor. */
  int selectBufferSize() const { return selectBufferSize_; }

  /*! Returns the width (in pixels) of a selection frustum, centered on the
  mouse cursor, that is used to select objects.

  The height of the selection frustum is defined by selectRegionHeight().

  The objects that will be drawn in this region by drawWithNames() will be
  recorded in the selectBuffer(). endSelection() then analyzes this buffer and
  setSelectedName() to the name of the closest object. See the gluPickMatrix()
  documentation for details.

  The default value is 3, which is adapted to standard applications. A smaller
  value results in a more precise selection but the user has to be careful for
  small feature selection.

  See the <a href="../examples/multiSelect.html">multiSelect example</a> for an
  illustration. */
  int selectRegionWidth() const { return selectRegionWidth_; }
  /*! See the selectRegionWidth() documentation. Default value is 3 pixels. */
  int selectRegionHeight() const { return selectRegionHeight_; }

  /*! Returns a pointer to an array of \c GLuint.

  This buffer is used by the \c GL_SELECT mode in select() to perform object
  selection. The buffer size can be modified using setSelectBufferSize(). If you
  overload endSelection(), you will analyze the content of this buffer. See the
  \c glSelectBuffer() man page for details. */
  GLuint *selectBuffer() { return selectBuffer_; }

public:
  virtual void select(const QMouseEvent *event);
  virtual void select(const QPoint &point);

  void setSelectBufferSize(int size);
  /*! Sets the selectRegionWidth(). */
  void setSelectRegionWidth(int width) { selectRegionWidth_ = width; }
  /*! Sets the selectRegionHeight(). */
  void setSelectRegionHeight(int height) { selectRegionHeight_ = height; }
  /*! Set the selectedName() value.

    Used in endSelection() during a selection. You should only call this method
    if you overload the endSelection() method. */
  void setSelectedName(int id) { selectedObjectId_ = id; }

protected:
  virtual void beginSelection(const QPoint &point);
  /*! This method is called by select() and should draw selectable entities.

  Default implementation is empty. Overload and draw the different elements of
your scene you want to be able to select. The default select() implementation
relies on the \c GL_SELECT, and requires that each selectable element is drawn
within a \c glPushName() - \c glPopName() block. A typical usage would be (see
the <a href="../examples/select.html">select example</a>): \code void
Viewer::drawWithNames() { for (int i=0; i<nbObjects; ++i) { glPushName(i);
    object(i)->draw();
    glPopName();
   }
}
\endcode

  The resulting selected name is computed by endSelection(), which
setSelectedName() to the integer id pushed by this method (a value of -1 means
no selection). Use selectedName() to update your selection, probably in the
postSelection() method.

  \attention If your selected objects are points, do not use \c
glBegin(GL_POINTS); and \c glVertex3fv() in the above \c draw() method (not
compatible with raster mode): use \c glRasterPos3fv() instead. */
  virtual void drawWithNames() {}
  virtual void endSelection(const QPoint &point);
  /*! This method is called at the end of the select() procedure. It should
  finalize the selection process and update the data
  structure/interface/computation/display... according to the newly selected
  entity.

  The default implementation is empty. Overload this method if needed, and use
  selectedName() to retrieve the selected entity name (returns -1 if no object
  was selected). See the <a href="../examples/select.html">select example</a>
  for an illustration. */
  virtual void postSelection(const QPoint &point) {  }
  //@}

  /*! @name Keyboard customization */
  //@{
public:
  /*! Defines the different actions that can be associated with a keyboard
  shortcut using setShortcut().

  See the <a href="../keyboard.html">keyboard page</a> for details. */
  enum KeyboardAction {
    DRAW_AXIS,
    DRAW_GRID,
    DISPLAY_FPS,
    ENABLE_TEXT,
    CAMERA_MODE,
    FULL_SCREEN,
    ANIMATION,
    HELP,
    EDIT_CAMERA,
    MOVE_CAMERA_LEFT,
    MOVE_CAMERA_RIGHT,
    MOVE_CAMERA_UP,
    MOVE_CAMERA_DOWN,
    INCREASE_FLYSPEED,
    DECREASE_FLYSPEED
  };

  unsigned int shortcut(KeyboardAction action) const;
#ifndef DOXYGEN
  // QGLViewer 1.x
  unsigned int keyboardAccelerator(KeyboardAction action) const;
  Qt::Key keyFrameKey(unsigned int index) const;
  Qt::KeyboardModifier playKeyFramePathStateKey() const;
  // QGLViewer 2.0 without Qt4 support
  Qt::KeyboardModifier addKeyFrameStateKey() const;
  Qt::KeyboardModifier playPathStateKey() const;
#endif
  Qt::Key pathKey(unsigned int index) const;
  Qt::KeyboardModifier addKeyFrameKeyboardModifiers() const;
  Qt::KeyboardModifier playPathKeyboardModifiers() const;

public:
  void setShortcut(KeyboardAction action, unsigned int key);
#ifndef DOXYGEN
  void setKeyboardAccelerator(KeyboardAction action, unsigned int key);
#endif
  void setKeyDescription(unsigned int key, std::string description);
  void clearShortcuts();

// Key Frames shortcut keys
#ifndef DOXYGEN
  // QGLViewer 1.x compatibility methods
  virtual void setKeyFrameKey(unsigned int index, int key);
  virtual void setPlayKeyFramePathStateKey(unsigned int buttonState);
  // QGLViewer 2.0 without Qt4 support
  virtual void setPlayPathStateKey(unsigned int buttonState);
  virtual void setAddKeyFrameStateKey(unsigned int buttonState);
#endif
  virtual void setPathKey(int key, unsigned int index = 0);
  virtual void setPlayPathKeyboardModifiers(Qt::KeyboardModifier modifiers);
  virtual void setAddKeyFrameKeyboardModifiers(Qt::KeyboardModifier modifiers);
  //@}

public:
  /*! @name Mouse customization */
  //@{
  /*! Defines the different mouse handlers: camera() or manipulatedFrame().

  Used by setMouseBinding(), setMouseBinding(Qt::KeyboardModifier modifiers,
  Qt::MouseButton, ClickAction, bool, int) and setWheelBinding() to define
  which handler receives the mouse events. */
  enum MouseHandler { CAMERA, FRAME };

  /*! Defines the possible actions that can be binded to a mouse click using
  setMouseBinding(Qt::KeyboardModifier, Qt::MouseButton, ClickAction, bool,
  int).

  See the <a href="../mouse.html">mouse page</a> for details. */
  enum ClickAction {
    NO_CLICK_ACTION,
    ZOOM_ON_PIXEL,
    ZOOM_TO_FIT,
    SELECT,
    RAP_FROM_PIXEL,
    RAP_IS_CENTER,
    CENTER_FRAME,
    CENTER_SCENE,
    SHOW_ENTIRE_SCENE,
    ALIGN_FRAME,
    ALIGN_CAMERA
  };

  /*! Defines the possible actions that can be binded to a mouse action (a
  click, followed by a mouse displacement).

  These actions may be binded to the camera() or to the manipulatedFrame() (see
  QGLViewer::MouseHandler) using setMouseBinding(). */
  enum MouseAction {
    NO_MOUSE_ACTION,
    ROTATE,
    ZOOM,
    TRANSLATE,
    MOVE_FORWARD,
    LOOK_AROUND,
    MOVE_BACKWARD,
    SCREEN_ROTATE,
    ROLL,
    DRIVE,
    SCREEN_TRANSLATE,
    ZOOM_ON_REGION
  };

#ifndef DOXYGEN
  MouseAction mouseAction(unsigned int state) const;
  int mouseHandler(unsigned int state) const;
  int mouseButtonState(MouseHandler handler, MouseAction action,
                       bool withConstraint = true) const;
  ClickAction clickAction(unsigned int state, bool doubleClick,
                          Qt::MouseButton buttonsBefore) const;
  void getClickButtonState(ClickAction action, unsigned int &state,
                           bool &doubleClick,
                           Qt::MouseButton &buttonsBefore) const;
  unsigned int wheelButtonState(MouseHandler handler, MouseAction action,
                                bool withConstraint = true) const;
#endif

  MouseAction mouseAction(Qt::Key key, Qt::KeyboardModifier modifiers,
                          Qt::MouseButton button) const;
  int mouseHandler(Qt::Key key, Qt::KeyboardModifier modifiers,
                   Qt::MouseButton button) const;

  void getMouseActionBinding(MouseHandler handler, MouseAction action,
                             bool withConstraint, Qt::Key &key,
                             Qt::KeyboardModifier &modifiers,
                             Qt::MouseButton &button) const;

  ClickAction clickAction(Qt::Key key, Qt::KeyboardModifier modifiers,
                          Qt::MouseButton button, bool doubleClick = false,
                          Qt::MouseButton buttonsBefore = Qt::NoButton) const;

  void getClickActionBinding(ClickAction action, Qt::Key &key,
                             Qt::KeyboardModifier &modifiers,
                             Qt::MouseButton &button, bool &doubleClick,
                             Qt::MouseButton &buttonsBefore) const;

  MouseAction wheelAction(Qt::Key key, Qt::KeyboardModifier modifiers) const;
  int wheelHandler(Qt::Key key, Qt::KeyboardModifier modifiers) const;

  void getWheelActionBinding(MouseHandler handler, MouseAction action,
                             bool withConstraint, Qt::Key &key,
                             Qt::KeyboardModifier &modifiers) const;

  void setMouseBinding(unsigned int state, MouseHandler handler,
                       MouseAction action, bool withConstraint = true);
  void setMouseBinding(unsigned int state, ClickAction action,
                       bool doubleClick = false,
                       Qt::MouseButton buttonsBefore = Qt::NoButton);
  void
  setMouseBindingDescription(unsigned int state, std::string description,
                             bool doubleClick = false,
                             Qt::MouseButton buttonsBefore = Qt::NoButton);

  void setMouseBinding(Qt::KeyboardModifier modifiers, Qt::MouseButton buttons,
                       MouseHandler handler, MouseAction action,
                       bool withConstraint = true);
  void setMouseBinding(Qt::KeyboardModifier modifiers, Qt::MouseButton button,
                       ClickAction action, bool doubleClick = false,
                       Qt::MouseButton buttonsBefore = Qt::NoButton);
  void setWheelBinding(Qt::KeyboardModifier modifiers, MouseHandler handler,
                       MouseAction action, bool withConstraint = true);
  void
  setMouseBindingDescription(Qt::KeyboardModifier modifiers,
                             Qt::MouseButton button, std::string description,
                             bool doubleClick = false,
                             Qt::MouseButton buttonsBefore = Qt::NoButton);

  void setMouseBinding(Qt::Key key, Qt::KeyboardModifier modifiers,
                       Qt::MouseButton buttons, MouseHandler handler,
                       MouseAction action, bool withConstraint = true);
  void setMouseBinding(Qt::Key key, Qt::KeyboardModifier modifiers,
                       Qt::MouseButton button, ClickAction action,
                       bool doubleClick = false,
                       Qt::MouseButton buttonsBefore = Qt::NoButton);
  void setWheelBinding(Qt::Key key, Qt::KeyboardModifier modifiers,
                       MouseHandler handler, MouseAction action,
                       bool withConstraint = true);
  void
  setMouseBindingDescription(Qt::Key key, Qt::KeyboardModifier modifiers,
                             Qt::MouseButton button, std::string description,
                             bool doubleClick = false,
                             Qt::MouseButton buttonsBefore = Qt::NoButton);

  void clearMouseBindings();

#ifndef DOXYGEN
  MouseAction wheelAction(Qt::KeyboardModifier modifiers) const;
  int wheelHandler(Qt::KeyboardModifier modifiers) const;

  void setHandlerKeyboardModifiers(MouseHandler handler,
                                   Qt::KeyboardModifier modifiers);
  void setHandlerStateKey(MouseHandler handler, unsigned int buttonState);
  void setMouseStateKey(MouseHandler handler, unsigned int buttonState);
#endif

private:
  static std::string mouseActionString(QGLViewer::MouseAction ma);
  static std::string clickActionString(QGLViewer::ClickAction ca);
  //@}


//@}

  /*! @name Visual hints */
  //@{
public:
  virtual void setVisualHintsMask(int mask, int delay = 2000);
  virtual void drawVisualHints();

public:
  virtual void resetVisualHints();
//@}

private:
  // Patch for a Qt bug with fullScreen on startup
  void delayedFullScreen() {
    //move(prevPos_);
    setFullScreen();
  }
  void hideMessage();

private:
  // Copy constructor and operator= are declared private and undefined
  // Prevents everyone from trying to use them
  QGLViewer(const QGLViewer &v);
  QGLViewer &operator=(const QGLViewer &v);

  // Set parameters to their default values. Called by the constructors.
  void defaultConstructor();

  void handleKeyboardAction(KeyboardAction id);

  // C a m e r a
  qglviewer::Camera *camera_;
  bool cameraIsEdited_;
  qreal previousCameraZClippingCoefficient_;
  unsigned int previousPathId_; // double key press recognition
  void connectAllCameraKFIInterpolatedSignals(bool connection = true);

  // C o l o r s
  QColor backgroundColor_, foregroundColor_;

  // D i s p l a y    f l a g s
  bool axisIsDrawn_;    // world axis
  bool gridIsDrawn_;    // world XY grid
  bool FPSIsDisplayed_; // Frame Per Seconds
  bool textIsEnabled_;  // drawText() actually draws text or not
  bool fullScreen_;     // full screen mode
  QPoint prevPos_;      // Previous window position, used for full screen mode

  // A n i m a t i o n
  bool animationStarted_; // animation mode started
  int animationPeriod_;   // period in msecs
  Metronom animationTimer_;

  // F P S    d i s p l a y
  Metronom fpsTime_;
  unsigned int fpsCounter_;
  std::string fpsString_;
  qreal f_p_s_;

  // M e s s a g e s
  std::string message_;
  bool displayMessage_;
  Metronom messageTimer_;

  Metronom delayedFullScreenTimer_;
  Metronom resetVisualHintsTimer_;

  // M a n i p u l a t e d    f r a m e
  qglviewer::ManipulatedFrame *manipulatedFrame_;
  bool manipulatedFrameIsACamera_;

  // M o u s e   G r a b b e r
  qglviewer::MouseGrabber *mouseGrabber_;
  bool mouseGrabberIsAManipulatedFrame_;
  bool mouseGrabberIsAManipulatedCameraFrame_;
  std::map<size_t, bool> disabledMouseGrabbers_;

  // S e l e c t i o n
  int selectRegionWidth_, selectRegionHeight_;
  int selectBufferSize_;
  GLuint *selectBuffer_;
  int selectedObjectId_;

  // V i s u a l   h i n t s
  int visualHint_;

  // S h o r t c u t   k e y s
  void setDefaultShortcuts();
  std::string cameraPathKeysString() const;
  std::map<KeyboardAction, std::string> keyboardActionDescription_;
  std::map<KeyboardAction, unsigned int> keyboardBinding_;
  std::map<unsigned int, std::string> keyDescription_;

  // K e y   F r a m e s   s h o r t c u t s
  std::map<Qt::Key, unsigned int> pathIndex_;
  Qt::KeyboardModifier addKeyFrameKeyboardModifiers_,
      playPathKeyboardModifiers_;

  // B u f f e r   T e x t u r e
  GLuint bufferTextureId_;
  qreal bufferTextureMaxU_, bufferTextureMaxV_;
  int bufferTextureWidth_, bufferTextureHeight_;
  unsigned int previousBufferTextureFormat_;
  int previousBufferTextureInternalFormat_;

  // M o u s e   a c t i o n s
  struct MouseActionPrivate {
    MouseHandler handler;
    MouseAction action;
    bool withConstraint;

    MouseActionPrivate operator=(const MouseActionPrivate &map) {
     
     handler = map.handler;
     action = map.action;
     withConstraint = map.withConstraint;
      return *this;
    }
  };

  // M o u s e   b i n d i n g s
  struct MouseBindingPrivate {
    const Qt::KeyboardModifier modifiers;
    const Qt::MouseButton button;
    const Qt::Key key;

    MouseBindingPrivate(Qt::KeyboardModifier m, Qt::MouseButton b, Qt::Key k)
        : modifiers(m), button(b), key(k) {}

    // This sort order is used in mouseString() to display sorted mouse bindings
    bool operator<(const MouseBindingPrivate &mbp) const {
      if (key != mbp.key)
        return key < mbp.key;
      if (modifiers != mbp.modifiers)
        return modifiers < mbp.modifiers;
      return button < mbp.button;
    }

  };

  // W h e e l   b i n d i n g s
  struct WheelBindingPrivate {
    const Qt::KeyboardModifier modifiers;
    const Qt::Key key;

    WheelBindingPrivate(Qt::KeyboardModifier m, Qt::Key k)
        : modifiers(m), key(k) {}

    // This sort order is used in mouseString() to display sorted wheel bindings
    bool operator<(const WheelBindingPrivate &wbp) const {
      if (key != wbp.key)
        return key < wbp.key;
      return modifiers < wbp.modifiers;
    }
  };

  // C l i c k   b i n d i n g s
  struct ClickBindingPrivate {
    const Qt::KeyboardModifier modifiers;
    const Qt::MouseButton button;
    const bool doubleClick;
    const Qt::MouseButton buttonsBefore; // only defined when doubleClick is true
    const Qt::Key key;

    ClickBindingPrivate(Qt::KeyboardModifier m, Qt::MouseButton b, bool dc,
                        Qt::MouseButton bb, Qt::Key k)
        : modifiers(m), button(b), doubleClick(dc), buttonsBefore(bb), key(k) {}

    // This sort order is used in mouseString() to display sorted mouse bindings
    bool operator<(const ClickBindingPrivate &cbp) const {
      if (key != cbp.key)
        return key < cbp.key;
      if (buttonsBefore != cbp.buttonsBefore)
        return buttonsBefore < cbp.buttonsBefore;
      if (modifiers != cbp.modifiers)
        return modifiers < cbp.modifiers;
      if (button != cbp.button)
        return button < cbp.button;
      return doubleClick < cbp.doubleClick;
    }
  };

  static std::string formatClickActionPrivate(ClickBindingPrivate cbp);
  static bool isValidShortcutKey(int key);

  std::map<ClickBindingPrivate, std::string> mouseDescription_;

  void setDefaultMouseBindings();
  void performClickAction(ClickAction ca, const QMouseEvent *const e);
  std::map<MouseBindingPrivate, MouseActionPrivate> mouseBinding_;
  std::map<WheelBindingPrivate, MouseActionPrivate> wheelBinding_;
  std::map<ClickBindingPrivate, ClickAction> clickBinding_;
  Qt::Key currentlyPressedKey_;

  // S n a p s h o t s
  TileRegion *tileRegion_;

};

#endif // QGLVIEWER_QGLVIEWER_H
