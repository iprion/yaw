#include "qglviewer.h" // for QGLViewer::drawAxis and Camera::drawCamera
#include <algorithm>    // std::for_each
#include <iterator>

using namespace qglviewer;
using namespace std;

/*! Creates a KeyFrameInterpolator, with \p frame as associated frame().

  The frame() can be set or changed using setFrame().

  interpolationTime(), interpolationSpeed() and interpolationPeriod() are set to
  their default values. */
KeyFrameInterpolator::KeyFrameInterpolator(Frame *frame)
    : Signaler({"interpolated", "endReached"}),
      frame_(nullptr), period_(40), interpolationTime_(0.0),
      interpolationSpeed_(1.0), interpolationStarted_(false),
      loopInterpolation_(false), pathIsValid_(false),
      valuesAreValid_(true), currentFrameValid_(false)
{
  setFrame(frame);
  for (int i = 0; i < 4; ++i)
    currentFrame_.push_back(keyFrame_.begin());
  timer_.connect("timeout", std::bind(&KeyFrameInterpolator::update, this), this);
}

/*! Virtual destructor. Clears the keyFrame path. */
KeyFrameInterpolator::~KeyFrameInterpolator() {
  deletePath();
}

/*! Sets the frame() associated to the KeyFrameInterpolator. */
void KeyFrameInterpolator::setFrame(Frame *const frame) {
  if (this->frame())
    this->disconnect("interpolated", this->frame());


  frame_ = frame;

  if (this->frame())
    this->connect("interpolated", this->frame()->signal("interpolated"), this->frame());
}

/*! Updates frame() state according to current interpolationTime(). Then adds
  interpolationPeriod()*interpolationSpeed() to interpolationTime().

  This internal method is called by a timer when interpolationIsStarted(). It
  can be used for debugging purpose. stopInterpolation() is called when
  interpolationTime() reaches firstTime() or lastTime(), unless
  loopInterpolation() is \c true. */
void KeyFrameInterpolator::update() {
  interpolateAtTime(interpolationTime());

  interpolationTime_ += interpolationSpeed() * interpolationPeriod() / 1000.0;

  if (interpolationTime() > keyFrame_.back()->time()) {
    if (loopInterpolation())
      setInterpolationTime(keyFrame_.front()->time() + interpolationTime_ -
                           keyFrame_.back()->time());
    else {
      // Make sure last KeyFrame is reached and displayed
      interpolateAtTime(keyFrame_.back()->time());
      stopInterpolation();
    }
    emit("endReached");
  } else if (interpolationTime() < keyFrame_.front()->time()) {
    if (loopInterpolation())
      setInterpolationTime(keyFrame_.back()->time() -
                           keyFrame_.front()->time() + interpolationTime_);
    else {
      // Make sure first KeyFrame is reached and displayed
      interpolateAtTime(keyFrame_.front()->time());
      stopInterpolation();
    }
    emit("endReached");
  }
}

/*! Starts the interpolation process.

  A timer is started with an interpolationPeriod() period that updates the
  frame()'s position and orientation. interpolationIsStarted() will return \c
  true until stopInterpolation() or toggleInterpolation() is called.

  If \p period is positive, it is set as the new interpolationPeriod(). The
  previous interpolationPeriod() is used otherwise (default).

  If interpolationTime() is larger than lastTime(), interpolationTime() is reset
  to firstTime() before interpolation starts (and inversely for negative
  interpolationSpeed()).

  Use setInterpolationTime() before calling this method to change the starting
  interpolationTime().

  See the <a href="../examples/keyFrames.html">keyFrames example</a> for an
  illustration.

  You may also be interested in QGLViewer::animate() and
  QGLViewer::startAnimation().

  \attention The keyFrames must be defined (see addKeyFrame()) \e before you
  startInterpolation(), or else the interpolation will naturally immediately
  stop. */
void KeyFrameInterpolator::startInterpolation(int period) {
  if (period >= 0)
    setInterpolationPeriod(period);

  if (!keyFrame_.empty()) {
    if ((interpolationSpeed() > 0.0) &&
        (interpolationTime() >= keyFrame_.back()->time()))
      setInterpolationTime(keyFrame_.front()->time());
    if ((interpolationSpeed() < 0.0) &&
        (interpolationTime() <= keyFrame_.front()->time()))
      setInterpolationTime(keyFrame_.back()->time());
    timer_.start(interpolationPeriod());
    interpolationStarted_ = true;
    update();
  }
}

/*! Stops an interpolation started with startInterpolation(). See
 * interpolationIsStarted() and toggleInterpolation(). */
void KeyFrameInterpolator::stopInterpolation() {
  timer_.stop();
  interpolationStarted_ = false;
}

/*! Stops the interpolation and resets interpolationTime() to the firstTime().

If desired, call interpolateAtTime() after this method to actually move the
frame() to firstTime(). */
void KeyFrameInterpolator::resetInterpolation() {
  stopInterpolation();
  setInterpolationTime(firstTime());
}

/*! Appends a new keyFrame to the path, with its associated \p time (in
  seconds).

  The keyFrame is given as a pointer to a Frame, which will be connected to the
  KeyFrameInterpolator: when \p frame is modified, the KeyFrameInterpolator path
  is updated accordingly. This allows for dynamic paths, where keyFrame can be
  edited, even during the interpolation. See the <a
  href="../examples/keyFrames.html">keyFrames example</a> for an illustration.

  \c nullptr \p frame pointers are silently ignored. The keyFrameTime() has to be
  monotonously increasing over keyFrames.

  Use addKeyFrame(const Frame&, qreal) to add keyFrame by values. */
void KeyFrameInterpolator::addKeyFrame(Frame * frame, qreal time) {
  if (!frame)
    return;

  if (keyFrame_.empty())
    interpolationTime_ = time;

  if ((!keyFrame_.empty()) && (keyFrame_.back()->time() > time))
    std::cerr << "Error in KeyFrameInterpolator::addKeyFrame: time is not monotone" << std::endl;
  else
    keyFrame_.push_back(new KeyFrame(frame, time));
  frame->connect("modified", std::bind(&KeyFrameInterpolator::invalidateValues, this), this);
  valuesAreValid_ = false;
  pathIsValid_ = false;
  currentFrameValid_ = false;
  resetInterpolation();
}

/*! Appends a new keyFrame to the path, with its associated \p time (in
  seconds).

  The path will use the current \p frame state. If you want the path to change
  when \p frame is modified, you need to pass a \e pointer to the Frame instead
  (see addKeyFrame(const Frame*, qreal)).

  The keyFrameTime() have to be monotonously increasing over keyFrames. */
void KeyFrameInterpolator::addKeyFrame(Frame &frame, qreal time) {
  if (keyFrame_.empty())
    interpolationTime_ = time;

  if ((!keyFrame_.empty()) && (keyFrame_.back()->time() > time))
    std::cerr << "Error in KeyFrameInterpolator::addKeyFrame: time is not monotone" << std::endl;
  else
    keyFrame_.push_back(new KeyFrame(frame, time));

  valuesAreValid_ = false;
  pathIsValid_ = false;
  currentFrameValid_ = false;
  resetInterpolation();
}

/*! Appends a new keyFrame to the path.

 Same as addKeyFrame(const Frame* frame, qreal), except that the keyFrameTime()
 is set to the previous keyFrameTime() plus one second (or 0.0 if there is no
 previous keyFrame). */
void KeyFrameInterpolator::addKeyFrame(Frame *frame) {
  qreal time;
  if (keyFrame_.empty())
    time = 0.0;
  else
    time = lastTime() + 1.0;

  addKeyFrame(frame, time);
}

/*! Appends a new keyFrame to the path.

 Same as addKeyFrame(const Frame& frame, qreal), except that the keyFrameTime()
 is automatically set to previous keyFrameTime() plus one second (or 0.0 if
 there is no previous keyFrame). */
void KeyFrameInterpolator::addKeyFrame(Frame &frame) {
  qreal time;
  if (keyFrame_.empty())
    time = 0.0;
  else
    time = keyFrame_.back()->time() + 1.0;

  addKeyFrame(frame, time);
}

/*! Removes all keyFrames from the path. The numberOfKeyFrames() is set to 0. */
void KeyFrameInterpolator::deletePath() {
  stopInterpolation();
  for(auto it = keyFrame_.begin() ; it!=keyFrame_.end() ; ++it)
    delete *it;
  keyFrame_.clear();
  pathIsValid_ = false;
  valuesAreValid_ = false;
  currentFrameValid_ = false;
}

static void drawCamera(qreal scale) {
  glDisable(GL_LIGHTING);

  const qreal halfHeight = scale * 0.07;
  const qreal halfWidth = halfHeight * 1.3;
  const qreal dist = halfHeight / tan(qreal(M_PI) / 8.0);

  const qreal arrowHeight = 1.5 * halfHeight;
  const qreal baseHeight = 1.2 * halfHeight;
  const qreal arrowHalfWidth = 0.5 * halfWidth;
  const qreal baseHalfWidth = 0.3 * halfWidth;

  // Frustum outline
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_LINE_STRIP);
  glVertex3d(-halfWidth, halfHeight, -dist);
  glVertex3d(-halfWidth, -halfHeight, -dist);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(halfWidth, -halfHeight, -dist);
  glVertex3d(-halfWidth, -halfHeight, -dist);
  glEnd();
  glBegin(GL_LINE_STRIP);
  glVertex3d(halfWidth, -halfHeight, -dist);
  glVertex3d(halfWidth, halfHeight, -dist);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(-halfWidth, halfHeight, -dist);
  glVertex3d(halfWidth, halfHeight, -dist);
  glEnd();

  // Up arrow
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // Base
  glBegin(GL_QUADS);
  glVertex3d(-baseHalfWidth, halfHeight, -dist);
  glVertex3d(baseHalfWidth, halfHeight, -dist);
  glVertex3d(baseHalfWidth, baseHeight, -dist);
  glVertex3d(-baseHalfWidth, baseHeight, -dist);
  glEnd();

  // Arrow
  glBegin(GL_TRIANGLES);
  glVertex3d(0.0, arrowHeight, -dist);
  glVertex3d(-arrowHalfWidth, baseHeight, -dist);
  glVertex3d(arrowHalfWidth, baseHeight, -dist);
  glEnd();
}

/*! Draws the path used to interpolate the frame().

  \p mask controls what is drawn: if (mask & 1) (default), the position path is
  drawn. If (mask & 2), a camera representation is regularly drawn and if (mask
  & 4), an oriented axis is regularly drawn. Examples:

  \code
  drawPath();  // Simply draws the interpolation path
  drawPath(3); // Draws path and cameras
  drawPath(5); // Draws path and axis
  \endcode

  In the case where camera or axis is drawn, \p nbFrames controls the number of
  objects (axis or camera) drawn between two successive keyFrames. When \p
  nbFrames=1, only the path KeyFrames are drawn. \p nbFrames=2 also draws the
  intermediate orientation, etc. The maximum value is 30. \p nbFrames should
  divide 30 so that an object is drawn for each KeyFrame. Default value is 6.

  \p scale (default=1.0) controls the scaling of the camera and axis drawing. A
  value of QGLViewer::sceneRadius() should give good results.

  See the <a href="../examples/keyFrames.html">keyFrames example</a> for an
  illustration.

  The color of the path is the current \c glColor().

  \attention The OpenGL state is modified by this method: GL_LIGHTING is
  disabled and line width set to 2. Use this code to preserve your current
  OpenGL state: \code glPushAttrib(GL_ALL_ATTRIB_BITS);
  drawPathModifyGLState(mask, nbFrames, scale);
  glPopAttrib();
  \endcode */
void KeyFrameInterpolator::drawPath(int mask, int nbFrames, qreal scale) {
  const int nbSteps = 30;
  if (!pathIsValid_) {
    path_.clear();

    if (keyFrame_.empty())
      return;

    if (!valuesAreValid_)
      updateModifiedFrameValues();

    if (keyFrame_.front() == keyFrame_.back())
      path_.push_back(Frame(keyFrame_.front()->position(),
                            keyFrame_.front()->orientation()));
    else {
      static Frame fr;
      KeyFrame *kf_[4];
      kf_[0] = keyFrame_.front();
      kf_[1] = kf_[0];
      int index = 1;
      kf_[2] = (index < keyFrame_.size()) ? *std::next(keyFrame_.begin(),index) : nullptr;
      index++;
      kf_[3] = (index < keyFrame_.size()) ? *std::next(keyFrame_.begin(),index) : nullptr;

      while (kf_[2]) {
        Vec diff = kf_[2]->position() - kf_[1]->position();
        Vec v1 = 3.0 * diff - 2.0 * kf_[1]->tgP() - kf_[2]->tgP();
        Vec v2 = -2.0 * diff + kf_[1]->tgP() + kf_[2]->tgP();

        // cout << kf_[0]->time() << " , " << kf_[1]->time() << " , " <<
        // kf_[2]->time() << " , " << kf_[3]->time() << endl;
        for (int step = 0; step < nbSteps; ++step) {
          qreal alpha = step / static_cast<qreal>(nbSteps);
          fr.setPosition(kf_[1]->position() +
                         alpha * (kf_[1]->tgP() + alpha * (v1 + alpha * v2)));
          fr.setOrientation(Quaternion::squad(kf_[1]->orientation(),
                                              kf_[1]->tgQ(), kf_[2]->tgQ(),
                                              kf_[2]->orientation(), alpha));
          path_.push_back(fr);
        }

        // Shift
        kf_[0] = kf_[1];
        kf_[1] = kf_[2];
        kf_[2] = kf_[3];
        index++;
        kf_[3] = (index < keyFrame_.size()) ? *std::next(keyFrame_.begin(),index) : nullptr;
      }
      // Add last KeyFrame
      path_.push_back(Frame(kf_[1]->position(), kf_[1]->orientation()));
    }
    pathIsValid_ = true;
  }

  if (mask) {
    glDisable(GL_LIGHTING);
    glLineWidth(2);

    if (mask & 1) {
      glBegin(GL_LINE_STRIP);
      for(std::list<Frame>::iterator fr=path_.begin() ; fr != path_.end() ; ++fr)
          glVertex3fv(fr->position());
      glEnd();
    }
    if (mask & 6) {
      int count = 0;
      if (nbFrames > nbSteps)
        nbFrames = nbSteps;
      qreal goal = 0.0;
      for(std::list<Frame>::iterator fr=path_.begin() ; fr != path_.end() ; ++fr)
        if ((count++) >= goal) {
          goal += nbSteps / static_cast<qreal>(nbFrames);
          glPushMatrix();
          glMultMatrixd(fr->matrix());
          if (mask & 2)
            drawCamera(scale);
          if (mask & 4)
            QGLViewer::drawAxis(scale / 10.0);
          glPopMatrix();
        }
    }
  }
}

void KeyFrameInterpolator::updateModifiedFrameValues() {
  Quaternion prevQ = keyFrame_.front()->orientation();
  KeyFrame *kf;
  for (int i = 0; i < keyFrame_.size(); ++i) {
    kf = *std::next(keyFrame_.begin(),i);
    if (kf->frame())
      kf->updateValuesFromPointer();
    kf->flipOrientationIfNeeded(prevQ);
    prevQ = kf->orientation();
  }

  KeyFrame *prev = keyFrame_.front();
  kf = keyFrame_.front();
  int index = 1;
  while (kf) {
    KeyFrame *next = (index < keyFrame_.size()) ? *std::next(keyFrame_.begin(),index) : nullptr;
    index++;
    if (next)
      kf->computeTangent(prev, next);
    else
      kf->computeTangent(prev, kf);
    prev = kf;
    kf = next;
  }
  valuesAreValid_ = true;
}

/*! Returns the Frame associated with the keyFrame at index \p index.

 See also keyFrameTime(). \p index has to be in the range
 0..numberOfKeyFrames()-1.

 \note If this keyFrame was defined using a pointer to a Frame (see
 addKeyFrame(const Frame* const)), the \e current pointed Frame state is
 returned. */
Frame KeyFrameInterpolator::keyFrame(int index) const {
  const KeyFrame *const kf = *std::next(keyFrame_.begin(),index);
  return Frame(kf->position(), kf->orientation());
}

/*! Returns the time corresponding to the \p index keyFrame.

 See also keyFrame(). \p index has to be in the range 0..numberOfKeyFrames()-1.
 */
qreal KeyFrameInterpolator::keyFrameTime(int index) const {
  return (*(std::next(keyFrame_.begin(),index)))->time();
}

/*! Returns the duration of the KeyFrameInterpolator path, expressed in seconds.

 Simply corresponds to lastTime() - firstTime(). Returns 0.0 if the path has
 less than 2 keyFrames. See also keyFrameTime(). */
qreal KeyFrameInterpolator::duration() const {
  return lastTime() - firstTime();
}

/*! Returns the time corresponding to the first keyFrame, expressed in seconds.

Returns 0.0 if the path is empty. See also lastTime(), duration() and
keyFrameTime(). */
qreal KeyFrameInterpolator::firstTime() const {
  if (keyFrame_.empty())
    return 0.0;
  else
    return keyFrame_.front()->time();
}

/*! Returns the time corresponding to the last keyFrame, expressed in seconds.

Returns 0.0 if the path is empty. See also firstTime(), duration() and
keyFrameTime(). */
qreal KeyFrameInterpolator::lastTime() const {
  if (keyFrame_.empty())
    return 0.0;
  else
    return keyFrame_.back()->time();
}

void KeyFrameInterpolator::updateCurrentKeyFrameForTime(qreal time) {
  // Assertion: times are sorted in monotone order.
  // Assertion: keyFrame_ is not empty

  // TODO: Special case for loops when closed path is implemented !!
  if (!currentFrameValid_)
    // Recompute everything from scrach
    currentFrame_[1] = keyFrame_.begin();

  while ((*std::next(currentFrame_[1]))->time() > time) {
    currentFrameValid_ = false;
    if (currentFrame_[1] == keyFrame_.begin())
      break;
    std::advance(currentFrame_[1], -1);
  }

  if (!currentFrameValid_)
    *currentFrame_[2] = *currentFrame_[1];

  while ((*std::next(currentFrame_[2]))->time() < time) {
    currentFrameValid_ = false;
    if (std::next(currentFrame_[2]) == keyFrame_.end())
      break;
    std::advance(currentFrame_[2], 1);
  }

  if (!currentFrameValid_) {
    currentFrame_[1] = currentFrame_[2];
    if ((currentFrame_[1] != keyFrame_.begin()) &&
        (time < (*std::next(currentFrame_[2]))->time()))
      std::advance(currentFrame_[1], -1);

    *currentFrame_[0] = *currentFrame_[1];
    if (currentFrame_[0] != keyFrame_.begin())
      std::advance(currentFrame_[0], -1);

    *currentFrame_[3] = *currentFrame_[2];
    if (std::next(currentFrame_[3]) != keyFrame_.end())
      std::advance(currentFrame_[3], 1);

    currentFrameValid_ = true;
    splineCacheIsValid_ = false;
  }

  // cout << "Time = " << time << " : " << currentFrame_[0]->peekNext()->time()
  // << " , " << currentFrame_[1]->peekNext()->time() << " , " <<
  // currentFrame_[2]->peekNext()->time() << " , " <<
  // currentFrame_[3]->peekNext()->time() << endl;
}

void KeyFrameInterpolator::updateSplineCache() {
  Vec delta = (*std::next(currentFrame_[2]))->position() -
              (*std::next(currentFrame_[1]))->position();
  v1 = 3.0 * delta - 2.0 * std::next(*currentFrame_[1])->tgP() -
       std::next(*currentFrame_[2])->tgP();
  v2 = -2.0 * delta + std::next(*currentFrame_[1])->tgP() +
       std::next(*currentFrame_[2])->tgP();
  splineCacheIsValid_ = true;
}

/*! Interpolate frame() at time \p time (expressed in seconds).
  interpolationTime() is set to \p time and frame() is set accordingly.

  If you simply want to change interpolationTime() but not the frame() state,
  use setInterpolationTime() instead.

  Emits the interpolated() signal and makes the frame() emit the
  Frame::interpolated() signal. */
void KeyFrameInterpolator::interpolateAtTime(qreal time) {
  setInterpolationTime(time);

  if ((keyFrame_.empty()) || (!frame()))
    return;

  if (!valuesAreValid_)
    updateModifiedFrameValues();

  updateCurrentKeyFrameForTime(time);

  if (!splineCacheIsValid_)
    updateSplineCache();

  qreal alpha;
  qreal dt = (*std::next(currentFrame_[2]))->time() - (*std::next(currentFrame_[1]))->time();
  if (dt == 0.0)
    alpha = 0.0;
  else
    alpha = (time - (*std::next(currentFrame_[1]))->time()) / dt;

  // Linear interpolation - debug
  // Vec pos = alpha*(currentFrame_[2]->peekNext()->position()) +
  // (1.0-alpha)*(currentFrame_[1]->peekNext()->position());
  Vec pos =
      (*std::next(currentFrame_[1]))->position() +
      alpha * ((*std::next(currentFrame_[1]))->tgP() + alpha * (v1 + alpha * v2));
  Quaternion q = Quaternion::squad(
      (*std::next(currentFrame_[1]))->orientation(),
      (*std::next(currentFrame_[1]))->tgQ(), (*std::next(currentFrame_[2]))->tgQ(),
      (*std::next(currentFrame_[2]))->orientation(), alpha);
  frame()->setPositionAndOrientationWithConstraint(pos, q);

  emit("interpolated");
}

//////////// KeyFrame private class implementation /////////
KeyFrameInterpolator::KeyFrame::KeyFrame(const Frame &fr, qreal t)
    : time_(t), frame_(nullptr) {
  p_ = fr.position();
  q_ = fr.orientation();
}

KeyFrameInterpolator::KeyFrame::KeyFrame(const Frame *fr, qreal t)
    : time_(t), frame_(fr) {
  updateValuesFromPointer();
}

void KeyFrameInterpolator::KeyFrame::updateValuesFromPointer() {
  p_ = frame()->position();
  q_ = frame()->orientation();
}

void KeyFrameInterpolator::KeyFrame::computeTangent(
    const KeyFrame *const prev, const KeyFrame *const next) {
  tgP_ = 0.5 * (next->position() - prev->position());
  tgQ_ = Quaternion::squadTangent(prev->orientation(), q_, next->orientation());
}

void KeyFrameInterpolator::KeyFrame::flipOrientationIfNeeded(
    const Quaternion &prev) {
  if (Quaternion::dot(prev, q_) < 0.0)
    q_.negate();
}
