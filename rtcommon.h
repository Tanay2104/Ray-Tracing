#ifndef RTCOMMON_H
#define RTCOMMON_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

// C++ std using
using std::make_shared;
using std::shared_ptr;
// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;
// Utility functions
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}
// Common Headers
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif