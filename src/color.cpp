#include "color.h"
inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0)
        return std::sqrt(linear_component);
    return 0;
}

void write_color(std::ofstream& out, const color& pixel_colour) {
    auto r = pixel_colour.x();
    auto g = pixel_colour.y();
    auto b = pixel_colour.z();
    // Applying linear to gamma transform for gamma 2
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Translating the [0, 1] component values to the byte range [0, 255]
    static const interval intensity(0.000, 0.999);
    short rbyte = short(255.999 * intensity.clamp(r));
    short gbyte = short(255.999 * intensity.clamp(g));
    short bbyte = short(255.999 * intensity.clamp(b));


    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}