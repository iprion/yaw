#ifndef QGLVIEWER_VEC_H
#define QGLVIEWER_VEC_H

#include <iostream>
#include <math.h>
#include <opengl/gl.h>

// Included by all files as vec.h is at the end of the include hierarchy
#include "config.h" // Specific configuration options.

namespace qglviewer {

/*! \brief The Vec class represents 3D positions and 3D vectors.
  \class Vec vec.h QGLViewer/vec.h

  Vec is used as a parameter and return type by many methods of the library. It
  provides classical algebraic computational methods and is compatible with
  OpenGL:

  \code
  // Draws a point located at 3.0 OpenGL units in front of the camera
  Vec pos = camera()->position() + 3.0 * camera()->viewDirection();
  glBegin(GL_POINTS);
  glVertex3fv(pos);
  glEnd();
  \endcode

  This makes of Vec a good candidate for representing positions and vectors in
  your programs. Since it is part of the \c qglviewer namespace, specify \c
  qglviewer::Vec or use the qglviewer namespace: \code using namespace
  qglviewer; \endcode

  <h3>Interface with other vector classes</h3>

  Vec implements a universal explicit converter, based on the \c [] \c operator.
  Everywhere a \c const \c Vec& argument is expected, you can use your own
  vector type instead, as long as it implements this operator (see the Vec(const
  C& c) documentation).

  See also the Quaternion and the Frame documentations.
  \nosubgrouping */
class Vec {


public:

  union {
    struct {
      qreal x, y, z;
    };
    qreal v_[3];
  };

  /*! @name Setting the value */
  //@{
  /*! Default constructor. Value is set to (0,0,0). */
  Vec() : x(0.0), y(0.0), z(0.0) {}

  /*! Standard constructor with the x, y and z values. */
  Vec(qreal X, qreal Y, qreal Z) : x(X), y(Y), z(Z) {}

  /*! Universal explicit converter from any class to Vec. You can use your own
vector class everywhere a \c const \c Vec& parameter is required, as long as it
implements the \c operator[ ]:

\code
class MyVec
{
  // ...
  qreal operator[](int i) const { returns x, y or z when i=0, 1 or 2; }
}

MyVec v(...);
camera()->setPosition(v);
\endcode

Note that standard vector types (STL, \c qreal[3], ...) implement this operator
and can hence be used in place of Vec. See also operator const qreal*() .*/
  template <class C> explicit Vec(const C &c) : x(c[0]), y(c[1]), z(c[2]) {}
  // Should NOT be explicit to prevent conflicts with operator<<.

  // ! Copy constructor
  // Vec(const Vec& v) : x(v.x), y(v.y), z(v.z) {}

  /*! Equal operator.
  Vec &operator=(const Vec &v) {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
  }
  */

  /*! Set the current value. May be faster than using operator=() with a
   * temporary Vec(x,y,z). */
  void setValue(qreal X, qreal Y, qreal Z) {
    x = X;
    y = Y;
    z = Z;
  }

  // Universal equal operator which allows the use of any type in place of Vec,
  // as long as the [] operator is implemented (v[0]=v.x, v[1]=v.y, v[2]=v.z).
  // template <class C>
  // Vec& operator=(const C& c)
  // {
  // x=c[0]; y=c[1]; z=c[2];
  // return *this;
  // }
  //@}

  /*! @name Accessing the value */
  //@{
  /*! Bracket operator, with a constant return value. \p i must range in [0..2].
   */
  qreal operator[](int i) const {
    return v_[i];
  }

  /*! Bracket operator returning an l-value. \p i must range in [0..2]. */
  qreal &operator[](int i) {
    return v_[i];
  }

  /*! Conversion operator returning the memory address of the vector.

Very convenient to pass a Vec pointer as a parameter to \c GLdouble OpenGL
functions: \code Vec pos, normal; glNormal3dv(normal); glVertex3dv(pos);
\endcode */
  operator const qreal *() const {
    return v_;
  }

  /*! Non const conversion operator returning the memory address of the vector.

Useful to pass a Vec to a method that requires and fills a \c qreal*, as
provided by certain libraries. */
  operator qreal *() {
    return v_;
  }

  /*! Conversion operator returning the memory address of the vector.

Very convenient to pass a Vec pointer as a \c float parameter to OpenGL
functions: \code Vec pos, normal; glNormal3fv(normal); glVertex3fv(pos);
\endcode
\note The returned float array is a static shared by all \c Vec instances. */
  operator const float *() const {
    static float *const result = new float[3];
    result[0] = (float)x;
    result[1] = (float)y;
    result[2] = (float)z;
    return result;
  }
  //@}

  /*! @name Algebraic computations */
  //@{
  /*! Returns the sum of the two vectors. */
  friend Vec operator+(const Vec &a, const Vec &b) {
    return Vec(a.x + b.x, a.y + b.y, a.z + b.z);
  }

  /*! Returns the difference of the two vectors. */
  friend Vec operator-(const Vec &a, const Vec &b) {
    return Vec(a.x - b.x, a.y - b.y, a.z - b.z);
  }

  /*! Unary minus operator. */
  friend Vec operator-(const Vec &a) { return Vec(-a.x, -a.y, -a.z); }

  /*! Returns the product of the vector with a scalar. */
  friend Vec operator*(const Vec &a, qreal k) {
    return Vec(a.x * k, a.y * k, a.z * k);
  }

  /*! Returns the product of a scalar with the vector. */
  friend Vec operator*(qreal k, const Vec &a) { return a * k; }

  /*! Returns the division of the vector with a scalar.

Too small \p k values are \e not tested (unless the library was compiled with
the "debug" Qt \c CONFIG flag) and may result in \c NaN values. */
  friend Vec operator/(const Vec &a, qreal k) {
    return Vec(a.x / k, a.y / k, a.z / k);
  }

  /*! Returns \c true only when the two vector are not equal (see operator==()).
   */
  friend bool operator!=(const Vec &a, const Vec &b) { return !(a == b); }

  /*! Returns \c true when the squaredNorm() of the difference vector is lower
   * than 1E-10. */
  friend bool operator==(const Vec &a, const Vec &b) {
    const qreal epsilon = 1.0E-10;
    return (a - b).squaredNorm() < epsilon;
  }

  /*! Adds \p a to the vector. */
  Vec &operator+=(const Vec &a) {
    x += a.x;
    y += a.y;
    z += a.z;
    return *this;
  }

  /*! Subtracts \p a to the vector. */
  Vec &operator-=(const Vec &a) {
    x -= a.x;
    y -= a.y;
    z -= a.z;
    return *this;
  }

  /*! Multiply the vector by a scalar \p k. */
  Vec &operator*=(qreal k) {
    x *= k;
    y *= k;
    z *= k;
    return *this;
  }

  /*! Divides the vector by a scalar \p k.

An absolute \p k value lower than 1E-10 will print a warning if the library was
compiled with the "debug" Qt \c CONFIG flag. Otherwise, no test is performed for
efficiency reasons. */
  Vec &operator/=(qreal k) {
    x /= k;
    y /= k;
    z /= k;
    return *this;
  }

  /*! Dot product of the two Vec. */
  friend qreal operator*(const Vec &a, const Vec &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
  }

  /*! Cross product of the two vectors. Same as cross(). */
  friend Vec operator^(const Vec &a, const Vec &b) { return cross(a, b); }

  /*! Cross product of the two Vec. Mind the order ! */
  friend Vec cross(const Vec &a, const Vec &b) {
    return Vec(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
               a.x * b.y - a.y * b.x);
  }

  Vec orthogonalVec() const;
//@}

/*! @name Norm of the vector */
//@{
  /*! Returns the \e squared norm of the Vec. */
  qreal squaredNorm() const { return x * x + y * y + z * z; }

  /*! Returns the norm of the vector. */
  qreal norm() const { return sqrt(x * x + y * y + z * z); }

  /*! Normalizes the Vec and returns its original norm.

Normalizing a null vector will result in \c NaN values. */
  qreal normalize() {
    const qreal n = norm();
    *this /= n;
    return n;
  }

  /*! Returns a unitary (normalized) \e representation of the vector. The
   * original Vec is not modified. */
  Vec unit() const {
    Vec v = *this;
    v.normalize();
    return v;
  }
  //@}

  /*! @name Projection */
  //@{
  void projectOnAxis(const Vec &direction);
  void projectOnPlane(const Vec &normal);
  //@}

};

} // namespace qglviewer

std::ostream &operator<<(std::ostream &o, const qglviewer::Vec &);

#endif // QGLVIEWER_VEC_H
