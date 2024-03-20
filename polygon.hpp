#ifndef POLY_H
#define POLY_H

#include "vec3.hpp"
#include "vertex.hpp"
#include "vec2.hpp"
#include "ray.hpp"
#include <math.h>
#include "shadingModel.hpp"
#include "hittable.hpp"
#include <vector>
#include "vertex.hpp"

using namespace std;

const double ZERO_BIAS = 0.0001;
const int SHADE_SMOOTH = 0;

class polygon : public hittable
{
    public:
        polygon(vector<shared_ptr<vertex>> vertices, shadingModel* material) 
        {
            this->material = material;
            this->normal = compute_normal(vertices);
            this->distance = compute_dist(vertices.at(0), normal);
            this->vertices = vertices;
            calculate_extents();
        }

        bool hit(const ray& cam_ray, double t_min, double t_max, hit_record& rec) override;

    private:
        shadingModel* material;
        point3 normal;
        double distance;
        vector<shared_ptr<vertex>> vertices;
        void calculate_extents() override;

        vec3 compute_normal(vector<shared_ptr<vertex>> vertices)
        {
            vec3 v1 = *vertices.at(1) - *vertices.at(0);
            vec3 v2 = *vertices.at(2) - *vertices.at(0);

            return unit_vector(cross(v1, v2));
        }

        double compute_dist(shared_ptr<vertex> point, vec3 normal)
        {
            double d = -(point->x() * normal.x()+ point->y() * normal.y() + point->z() * normal.z());
            return d;
        }

};

bool polygon::hit(const ray& cam_ray, double t_min, double t_max, hit_record& rec)
{
    //supporting plane tests
    auto denom = dot(normal,cam_ray.direction());
    
    if (denom < ZERO_BIAS && denom > -ZERO_BIAS)
    {
        return false;
    }

    auto d = dot(normal, *vertices.at(0));
    auto t = (d - dot(normal, cam_ray.origin())) / denom;

    if (t < t_min || t > t_max)
    {
        return false;
    }

    auto Q = cam_ray.at(t);

    //inside triangle tests and barycentric numerators

    auto BA = *vertices.at(1) - *vertices.at(0);

    auto numer_1 = dot(cross(BA, (Q - *vertices.at(0))), normal);
    auto numer_2 = dot(cross((*vertices.at(2) - *vertices.at(1)), (Q - *vertices.at(1))), normal);
    auto numer_3 = dot(cross((*vertices.at(0) - *vertices.at(2)), (Q - *vertices.at(2))), normal);

    if (numer_1 < 0)
    {
        return false;
    }
    if (numer_2 < 0)
    {
        return false;
    }
    if (numer_3 < 0)
    {
        return false;
    }
    
    auto bary_denom = dot(cross(BA, (*vertices.at(2) - *vertices.at(0))), normal);

    auto alpha = numer_2 / bary_denom;
    auto beta = numer_3 / bary_denom;
    auto gamma = numer_1 / bary_denom;

    if (SHADE_SMOOTH)
    {
        auto bary_norm = (alpha * vertices.at(0)->n) + (beta * vertices.at(1)->n) + (gamma * vertices.at(2)->n);
        normal = bary_norm/bary_norm.length();
    }

    rec.set_face_normal(cam_ray, normal);
    rec.d = cam_ray.direction();
    rec.t = t;
    rec.p = Q;
    rec.material = material;
    rec.uv = alpha * vertices.at(0)->uv + beta * vertices.at(1)->uv + gamma * vertices.at(2)->uv;

    return true;
}



void polygon::calculate_extents()
{
    double max_x = 0;
    double max_y = 0;
    double max_z = 0;

    double min_x = 0;
    double min_y = 0;
    double min_z = 0;

    for (shared_ptr<point3> point : vertices)
    {
        //x component
        if (point->x() > max_x)
        {
            max_x = point->x();
        }
        else if(point->x() < min_x)
        {
            min_x = point->x();
        }

        //y component
        if (point->y() > max_y)
        {
            max_y = point->y();
        }
        else if(point->y() < min_y)
        {
            min_y = point->y();
        }

        //z component
        if (point->z() > max_z)
        {
            max_z = point->z();
        }
        else if(point->z() < min_z)
        {
            min_z = point->z();
        }
    } 

    this->lower_extent = point3(min_x, min_y, min_z);
    this->upper_extent = point3(max_x, max_y, max_z);
}

#endif