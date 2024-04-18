#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtw_stb_image.h"
#include "vec2.hpp"
#include "vec3.hpp"

class texture
{
    public:
        virtual ~texture() = default;

        virtual color value(double u, double v) const = 0;
};

class image_texture : public texture
{
    public:
            image_texture(const char* filename) : image(filename) {}

            color value(double u, double v) const
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

class solid_color : public texture
{
    public:
        solid_color(color c) {solid_value = c;}
        solid_color(double red, double green, double blue) : solid_color(color(red,green,blue)) {}


        color value(double u, double v) const override 
        {
            return solid_value;
        }

    private:
        color solid_value;

};

class environment_color : image_texture
{
    public:
        environment_color(const char* filename) : image_texture(filename) {hasMap = true;}
        environment_color(color bg) : image_texture("") {this->bg = bg;}

        ~environment_color() {}

        color value(vec3 dir)
        {
            if (hasMap)
            {
                //uv coordinates
                auto theta = acos(-dir.y());
                auto phi = atan2(-dir.z(), dir.x()) + pi;

                auto u = phi / (2*pi);
                auto v =  1 - (theta / pi);

                return image_texture::value(u,v);

            }
            else
            {
                return bg;
            }

        }

    private:
        bool hasMap = false;
        color bg;
        rtw_image image;

};


#endif