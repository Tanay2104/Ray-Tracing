#ifndef COLOR_H
#define COLOR_H
#include "interval.h"
#include "vec3.h"
using color = vec3;
inline double linear_to_gamma(double linear_component);
void write_color(std::ofstream& out, const color& pixel_colour);
#endif