#ifndef VERTEX_H
#define VERTEX_H

#include "vec3.hpp"
#include "vertex.hpp"
#include "vec2.hpp"
#include "ray.hpp"
#include <math.h>
#include "shadingModel.hpp"
#include "hittable.hpp"
#include <vector>

using namespace std;

class vertex : public point3
{
    public:
        vertex(double x, double y, double z, double u, double v, vec3 n)
        {
            this->o[0] = x;
            this->o[1] = y;
            this->o[2] = z;
            this->uv[0] = u;
            this->uv[1] = v;
            this->n = n;
        }

        vertex(double x, double y, double z)
        {
            this->o[0] = x;
            this->o[1] = y;
            this->o[2] = z;
        }

        vertex(point3 point, vec2 uv, vec3 n)
        {
            this->o[0] = point.x();
            this->o[1] = point.y();
            this->o[2] = point.z();
            this->uv[0] = uv[0]; 
            this->uv[1] = 1-uv[1]; 
            this->n = n;
        }

        vec2 uv;
        vec3 n;
};

#endif
