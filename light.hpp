#ifndef LIGHT_H
#define LIGHT_H

#include "vec3.hpp"
#include "color.hpp"

class light
{

    public:
        virtual vec3 get_light_direction(point3 P) {}
        virtual float intensity(point3 P) {}
        color col = color(1,1,1);

        light() {}
        bool isArea = false;
        int samples = 0;

};

#endif
