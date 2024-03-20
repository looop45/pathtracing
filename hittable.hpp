#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.hpp"
#include "vec2.hpp"

class hit_record 
{
    public:
        point3 p;
        vec3 normal;
        double t;
        vec2 uv;
        shadingModel* material;
        bool front_face;
        vec3 d;
        int id;

        inline void set_face_normal(const ray& r, const vec3& outward_normal) 
        {
            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal :-outward_normal;
        }

    
};

class hittable {
    public:
        virtual bool hit(const ray& cam_ray, double t_min, double t_max, hit_record& rec) = 0;

        point3 get_lower_extent()  { return this->lower_extent; }
        point3 get_upper_extent()  { return this->upper_extent; } 
    protected:
        point3 lower_extent;
        point3 upper_extent;
        virtual void calculate_extents() = 0;

};

#endif
