#ifndef COLOR_H
#define COLOR_H
#include "interval.h"
#include "vec3.h"
using color = vec3;
void write_color(std::ostream& out, const color& pixel_colour) {
    auto r = pixel_colour.x();
    auto g = pixel_colour.y();
    auto b = pixel_colour.z();

    // Translating the [0, 1] component values to the byte range [0, 255]
    static const interval intensity(0.000, 0.999);
    short rbyte = short(255.999 * intensity.clamp(r));
    short gbyte = short(255.999 * intensity.clamp(g));
    short bbyte = short(255.999 * intensity.clamp(b));


    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif