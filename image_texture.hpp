#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H

#include "rtw_stb_image.h"
#include "vec2.hpp"
#include "vec3.hpp"

class image_texture
{
    public:
        image_texture(const char* filename) : image(filename) {}

        color value(double u, double v)
        {
            //no texture data
            if (image.height() <= 0) return color(0,1,1);

            //clamp input texture coordinates to [0,1] x [1,0]
            auto i = static_cast<int>(u * image.width());
            auto j = static_cast<int>(v * image.height());
            auto pixel = image.pixel_data(i,j);

            auto color_scale = 1.0 / 255.0;
            return color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
        }

    private:
        rtw_image image;
};


#endif