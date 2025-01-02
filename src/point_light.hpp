#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "light.hpp"
#include "vec3.hpp"

using namespace std;

class point_light : public light
{
    public:
        point_light(point3 P, color col, float brightness) 
        { 
            this->col = col; 
            this->brightness = brightness;
            this->P = P;
        }

        vec3 get_light_direction(point3 intersect)
        {
            return unit_vector(P-intersect);
        }

        float intensity(point3 intersect)
        {   
            vec3 dir = P-intersect;
            double dist_squared = dir.length_squared();
            return brightness/dist_squared;
        }

    private:
        point3 P;
        float brightness;


};

#endif