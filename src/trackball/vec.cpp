#include "vec.h"

// Most of the methods are declared inline in vec.h

using namespace qglviewer;
using namespace std;

/*! Projects the Vec on the axis of direction \p direction that passes through
the origin.

\p direction does not need to be normalized (but must be non null). */
void Vec::projectOnAxis(const Vec &direction) {
  *this = (((*this) * direction) / direction.squaredNorm()) * direction;
}

/*! Projects the Vec on the plane whose normal is \p normal that passes through
the origin.

\p normal does not need to be normalized (but must be non null). */
void Vec::projectOnPlane(const Vec &normal) {
  *this -= (((*this) * normal) / normal.squaredNorm()) * normal;
}

/*! Returns a Vec orthogonal to the Vec. Its norm() depends on the Vec, but is
 zero only for a null Vec. Note that the function that associates an
 orthogonalVec() to a Vec is not continous. */
Vec Vec::orthogonalVec() const {
  // Find smallest component. Keep equal case for null values.
  if ((fabs(y) >= 0.9 * fabs(x)) && (fabs(z) >= 0.9 * fabs(x)))
    return Vec(0.0, -z, y);
  else if ((fabs(x) >= 0.9 * fabs(y)) && (fabs(z) >= 0.9 * fabs(y)))
    return Vec(-z, 0.0, x);
  else
    return Vec(-y, x, 0.0);
}


ostream &operator<<(ostream &o, const Vec &v) {
  return o << v.x << '\t' << v.y << '\t' << v.z;
}
