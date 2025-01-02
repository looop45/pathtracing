#ifndef CAM_H
#define CAM_H

#include "vec3.hpp"
#include "ray.hpp"
#include <math.h>

class camera
{
    public:
        camera() {}
        camera(point3& org, vec3& dir, float& fov, const int& w, const int& h)
        {
            origin = org;
            direction = dir;
            fieldOfView = fov;
            width = w;
            height = h;

            float pi = 3.13159;

            plane_dist = cosf((fieldOfView/2) * (pi/180));
            plane_width = sinf((fieldOfView/2) * (pi/180));
            plane_height = plane_width;
        }

        //returns a ray given the x and y coordinate in screen space
        ray getRay(float x_coord, float y_coord)
        {
            vec3 dest((float)x_coord * plane_width/width - plane_width/2, (float)y_coord * plane_height/height - plane_height/2, plane_dist);

            vec3 dir = unit_vector(dest - origin);

            ray outRay(origin, dir);

            return outRay;
        }

    private:
        point3 origin;
        vec3 direction;
        float fieldOfView;
        int width;
        int height;
        float plane_dist;
        float plane_width;
        float plane_height;

};

#endif