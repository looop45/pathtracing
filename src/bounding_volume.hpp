#ifndef BOUNDING_H
#define BOUNDING_H

#include "utilities.hpp"
#include "hittable.hpp"
#include "material.hpp"

class bounding_volume : public hittable
{
    public:
        bounding_volume() {this->lower_extent = point3(0,0,0); this->upper_extent = point3(0,0,0); }
        bounding_volume(point3 lower_extent, point3 upper_extent) {this->lower_extent = lower_extent; this->upper_extent = upper_extent;}

        bool hit(const ray& cam_ray, double t_min, double t_max, hit_record& rec) override;
        bool hit(const ray& cam_ray, double t_min, double t_max, hit_record& rec, color c);
        void calculate_extents() override {} 

};

bool bounding_volume::hit(const ray& cam_ray, double t_min, double t_max, hit_record& rec)
{
    float tmin = (lower_extent.x() - cam_ray.origin().x()) / cam_ray.direction().x();
    float tmax = (upper_extent.x() - cam_ray.origin().x()) / cam_ray.direction().x();

    if (tmin > tmax) swap(tmin, tmax);
    vec3 normal(1,0,0);

    float tymin = (lower_extent.y() - cam_ray.origin().y()) / cam_ray.direction().y();
    float tymax = (upper_extent.y() - cam_ray.origin().y()) / cam_ray.direction().y();

    if (tymin > tymax) swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
    {
        return false;
    }

    if (tymin > tmin)
    {
        tmin = tymin;
        normal = vec3(0,1,0);
    }

    if (tymax < tmax)
    {
        tmax = tymax;
    }

    float tzmin = (lower_extent.z() - cam_ray.origin().z()) / cam_ray.direction().z();
    float tzmax = (upper_extent.z() - cam_ray.origin().z()) / cam_ray.direction().z();

    if (tzmin > tzmax) swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
    {
        return false;
    }

    if (tzmin > tmin)
    {
        tmin = tzmin;
        normal = vec3(0,0,1);

    }

    if (tzmax < tmax)
    {
        tmax = tzmax;
    }
    rec.t = tmin;
    rec.p = cam_ray.at(rec.t);
    rec.set_face_normal(cam_ray, normal);
    rec.d = cam_ray.direction();

    return true;
}

#endif
