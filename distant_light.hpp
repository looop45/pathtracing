#ifndef DISTANT_LIGHT_H
#define DISTANT_LIGHT_H

#include "light.hpp"

using namespace std;

class distant_light : public light
{
    public:
        distant_light(vec3 dir, color col, float brightness) 
        { 
            this->dir = dir;
            this->col = col; 
            this->brightness = brightness;
        }

        vec3 get_light_direction(point3 P)
        {
            return -dir;
        }

        float intensity(point3 intersect)
        {
            return brightness;
        }

    private:
        vec3 dir;
        float brightness;

};

#endif