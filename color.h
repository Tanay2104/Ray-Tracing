#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

using color = vec3;
void write_color(std::ostream& out, const color& pixel_colour) {
    auto r = pixel_colour.x();
    auto g = pixel_colour.y();
    auto b = pixel_colour.z();

    short rbyte = short(255.999 * r);
    short gbyte = short(255.999 * g);
    short bbyte = short(255.999 * b);


    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif