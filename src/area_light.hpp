#ifndef AREA_LIGHT_H
#define AREA_LIGHT_H

#include <random>
#include "light.hpp"
#include "vec3.hpp"
#include "quad.hpp"

using namespace std;

std::random_device rd1;
std::mt19937 gen1(rd1());
std::uniform_real_distribution<float> pos_distribution(-1.0f, 1.0f);

class area_light : public light
{
    public:
        area_light(point3 Q, vec3 u, vec3 v, color col, double brightness, int samples)
        {
            this->geo = quad(Q, u, v);
            this->col = col;
            this->brightness = brightness;
            this->samples = samples;
            isArea = true;
        }

        vec3 get_light_direction(point3 intersect)
        {
            light_pos = (geo.u/samples) * (i + pos_distribution(gen1)/samples) + (geo.v/samples) * (j + pos_distribution(gen1)/samples) + geo.Q;
            if (i == samples)
            {
                this->i = 0;
                this->j++;
                if (j == samples)
                {
                    this-> j = 0;
                }
            }
            else
            {
                i++;
            }
            

            return unit_vector(light_pos - intersect);
        }

        float intensity(point3 intersect)
        {   
            vec3 dir = light_pos-intersect;
            double dist_squared = dir.length_squared();
            return brightness/dist_squared;
        }

    private:
        quad geo;
        color col;
        double brightness;
        point3 light_pos;
        int i = 0;
        int j = 0;

};

#endif
