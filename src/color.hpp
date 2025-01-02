#ifndef COLOR_H
#define COLOR_H

#include "vec3.hpp"

#include <iostream>

void write_color(std::ostream &out, color pixel_color) {
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

color clamp(color c)
        {
            color out = c;
            if (c.x() > 1)
            {
                out[0] = 1;
            }
            if (c.y() > 1)
            {
                out[1] = 1;
            }
            if (c.z() > 1)
            {
                out[2] = 1;
            }
            if (c.x() < 0)
            {
                out[0] = 0.0;
            }
            if (c.y() < 0)
            {
                out[1] = 0.0;
            }
            if (c.z() < 0)
            {
                out[2] = 0.0;
            }

            return out;
        }

#endif