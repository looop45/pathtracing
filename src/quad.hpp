#ifndef QUAD_H
#define QUAD_H

#include "utilities.hpp"
#include <math.h>
#include "material.hpp"
#include "hittable.hpp"

class quad : public hittable
{
    public:
        quad(point3 Q, vec3 u, vec3 v, shared_ptr<material> mat)
        {
            this->Q = Q;
            this->u = u;
            this->v = v;

            this->mat = mat;

            auto n = cross(u,v);
            normal = unit_vector(n);
            this->w = n / dot(n,n);
            D = dot(normal, Q);

            area = n.length();

            this->calculate_extents();
        }
        
        quad(point3 Q, vec3 u, vec3 v)
        {
            this->Q = Q;
            this->u = u;
            this->v = v;

            auto n = cross(u,v);
            normal = unit_vector(n);
            this->w = n / dot(n,n);
            this->D = dot(normal, Q);
        }

        quad() {}

        double pdf_value(const point3& origin, const vec3& v) override 
        {
            hit_record rec;
            if (!this->hit(ray(origin, v), 0, infinity, rec))
                return 0;

            auto distance_squared = rec.t * rec.t * v.length_squared();
            auto cosine = fabs(dot(v, rec.normal) / v.length());

            return distance_squared / (cosine * area);
        }

        vec3 random(const point3& origin) override 
        {
            auto p = Q + (random_double() * u) + (random_double() * v);
            return p - origin;
        }

        vec3 u;
        vec3 v;
        point3 Q;
        double area;


        bool hit(const ray& cam_ray, double t_min, double t_max, hit_record& rec) override;

    private:
        void calculate_extents() override;
        vec3 normal;
        double D;
        vec3 w; 
        shared_ptr<material> mat;


};

bool quad::hit(const ray& cam_ray, double t_min, double t_max, hit_record& rec) 
{
    double denom = dot(this->normal, cam_ray.direction());

    // No hit if the ray is parallel to the plane.
    if (fabs(denom) < 1e-8)
        return false;

    // Return false if the hit point parameter t is outside the ray interval.
    auto t = (this->D - dot(normal, cam_ray.origin())) / denom;
    if (t < t_min || t > t_max)
        return false;

    auto intersection = cam_ray.at(t);
    vec3 planar_hitpt_vector = intersection - Q;

    auto alpha = dot(w, cross(planar_hitpt_vector, v));
    auto beta = dot(w, cross(u, planar_hitpt_vector));

    if ((alpha < 0) || (1 < alpha) || (beta < 0) || (1 < beta))
        return false;

    rec.t = t;
    rec.uv = vec2(alpha, 1-beta);
    rec.p = intersection;
    rec.mat = mat;
    rec.set_face_normal(cam_ray, normal);

    return true;
}

void quad::calculate_extents()
{
    const double delta = 0.0001;
    this->lower_extent = Q;
    this-> upper_extent = Q+u+v;

}


#endif