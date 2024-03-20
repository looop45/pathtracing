#ifndef TREE_H
#define TREE_H

#include "color.hpp"
#include "ray.hpp"
#include "light.hpp"
#include "vec3.hpp"

#include <memory>
#include <vector>
#include<random>


using namespace std;

const double infinity = numeric_limits<double>::infinity();
// Seed the random number generator
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

struct hit_node 
{
    hit_record rec;
    ray r;
    vector<hit_node*> reflection_ray;
    vector<hit_node*> transmission_ray;
    hit_node* parent = nullptr;

    hit_node(hit_record rec, ray r);
    hit_node() {}

    hit_node* get_reflection_ray(int depth, int refl_samples, int refr_samples, hittable& world, double roughness);
    hit_node* get_transmission_ray(int depth, int refl_samples, int refr_samples, hittable& world, double roughness);
};

class hit_tree
{
    public:
        hit_tree(ray r, hit_record rec, hittable& world, vector<shared_ptr<light> > lights, color bgColor, int refl_samples, int refr_samples) : world(world), lights(lights)
        {
            const int DEPTH = 3;

            root.r = r;
            root.rec = rec;
            this->world = world;
            this->lights = lights;
            this->bgColor = bgColor;
            this->refl_samples = refl_samples;
            this->refr_samples = refr_samples;

            construct_tree(DEPTH, root);
        }

        ~hit_tree() {}

        color evaluate();
        color node_solve(hit_node* node);

    private:
        hit_node root;
        hittable& world;
        vector<shared_ptr<light> > lights;
        color bgColor;

        int refl_samples;
        int refr_samples;

        hit_node* construct_tree(int depth, hit_node &root);
};

color hit_tree::evaluate()
{
    //DFS evaluation of color at each hit record, combined to final pixel color
    color sample_color = node_solve(&root);
    return sample_color;

}

color hit_tree::node_solve(hit_node* node)
{
    color out = color(0,0,0);
    color refl_color = color(0,0,0);
    color trans_color = color(0,0,0);

    //evaluate reflection component
    if (node->reflection_ray.size() != 0)
    {
        for (auto refl_node : node->reflection_ray)
        {
            if (refl_node != nullptr)
            {
                refl_color += node_solve(refl_node);
            }
        }
        refl_color = refl_color / node->reflection_ray.size();
    }
    else
    {
        refl_color = bgColor;
    }
    refl_color = refl_color * node->rec.material->refl;

    //evaluate transmission component
    if (node->transmission_ray.size() != 0)
    {
        for (auto trans_node : node->transmission_ray)
        {
            if (trans_node != nullptr)
            {
                trans_color = node_solve(trans_node);
            }
        }
        trans_color = trans_color;// / node->transmission_ray.size();
    }
    else
    {
        trans_color = bgColor;
    }
    trans_color = trans_color * node->rec.material->Kt;

    //diffuse spec component
    for (const auto& light : lights)
        {


            point3 p = node->rec.p;
            if (light->isArea)
            {
                for (int i = 0; i < pow(light->samples, 2); i++)
                {
                    //get the light's direction
                    vec3 light_direction = unit_vector(light->get_light_direction(node->rec.p));
                    ray shadow_ray(p, light_direction);
                    hit_record shadow_hit;
                    //cout << light->samples << endl;
                    if(!world.hit(shadow_ray, 0.001, infinity, shadow_hit))
                    {
                        color diffuse_lobe = node->rec.material->compute_diffuse(node->rec.normal, node->r.direction(), light_direction, light->col, node->rec.uv);
                        color spec_lobe = node->rec.material->compute_spec(node->rec.normal, node->r.direction(), light_direction, light->col);
                        out += (light->intensity(p) * (diffuse_lobe + spec_lobe)) / pow(light->samples, 2);
                    }
                }
                
            }
            else
            {
                //get the light's direction
                vec3 light_direction = unit_vector(light->get_light_direction(node->rec.p));
                ray shadow_ray(p, light_direction);
                hit_record shadow_hit;

                if(!world.hit(shadow_ray, 0.001, infinity, shadow_hit))
                {
                    color diffuse_lobe = node->rec.material->compute_diffuse(node->rec.normal, node->r.direction(), light_direction, light->col, node->rec.uv);
                    color spec_lobe = node->rec.material->compute_spec(node->rec.normal, node->r.direction(), light_direction, light->col);
                    out += light->intensity(p) * (diffuse_lobe + spec_lobe);

                }
            }
        }
    color ambient_term = node->rec.material->compute_ambient(node->rec.uv);
    
    return clamp(out + refl_color + ambient_term + trans_color);
}

hit_node* hit_tree::construct_tree(int depth, hit_node& node)
{

    //reflections
    if(node.rec.material->is_reflective())
    {
        for (int i = 0; i < refl_samples; i++)
        {
            node.reflection_ray.push_back(node.get_reflection_ray(depth, refl_samples, refr_samples, world, node.rec.material->roughness));
        }
    }
    //refraction
    if (node.rec.material->is_transmissive())
    {
        for (int i = 0; i < refl_samples; i++)
        {
            node.transmission_ray.push_back(node.get_transmission_ray(depth, refl_samples, refr_samples, world, node.rec.material->refr_rough));
        }
    }
    //hit_node* refraction_node = node.get

    //node.refraction_ray = construct_tree(depth-1, reflection_node);

}

hit_node* hit_node::get_transmission_ray(int depth, int refl_samples, int refr_samples, hittable& world, double rough)
{
    if (depth <= 0 || !this->rec.material->is_transmissive())
    {
        return nullptr;
    }

    double n2 = this->rec.material->IOR;
    double n1 = 1;

    if (this-> parent != nullptr && this->parent->rec.material->is_transmissive())
    {
        n1 = this->parent->rec.material->IOR;
        n2 = 1;
    }

    double nR = n1/n2;
    vec3 N = this->rec.normal;
    vec3 I = -this->r.direction();

    vec3 T_dir = (nR * (dot(N, I)) - sqrt(1-pow(nR, 2) * (1 - pow(dot(N,I),2)))) * N - nR * I;

    //refraction roughness
    vec3 jitter(distribution(gen), distribution(gen), distribution(gen));
    T_dir += jitter * rough;

    ray T = ray(this->rec.p, T_dir);

    hit_record rec;

    if (world.hit(T, 0.001, infinity, rec))
    {
        hit_node* trans_node = new hit_node();
        trans_node->parent = this;
        trans_node->r = T;
        trans_node->rec = rec;

        //refraction rays

        for (int i = 0; i < refr_samples; i++)
        {
            hit_node* transmission = trans_node->get_transmission_ray(depth-1, refl_samples, refr_samples, world, trans_node->rec.material->refr_rough);
            if (transmission != nullptr)
            {
                trans_node->transmission_ray.push_back(transmission);
            }
        }
        //secondary reflections
        if (rec.material->is_reflective())
        {
            for (int i = 0; i < refl_samples; i++)
            {
                hit_node* reflection = trans_node->get_reflection_ray(depth-1, refl_samples, refr_samples, world, trans_node->rec.material->roughness);
                if (reflection != nullptr)
                {
                    trans_node->reflection_ray.push_back(reflection);
                }
            }
        }
        return trans_node;
    }
    else
    {
        return nullptr;
    }
}

hit_node* hit_node::get_reflection_ray(int depth, int refl_samples, int refr_samples, hittable& world, double rough)
{   
;
    //std::uniform_real_distribution<float> pos_dist(0.0f, 1.0f);

    //base case, depth reached, none-reflective
    if (depth <= 0 || !this->rec.material->is_reflective())
    {
        return nullptr;
    }
    vec3 jitter(distribution(gen), distribution(gen), distribution(gen));
    //jitter = vec3(0,0,0);

    vec3 d = unit_vector(this->r.direction() + jitter * rough);
    vec3 n = this->rec.normal;
    vec3 r = d - 2 * n * (dot(d, n));
    //r = this->rec.p - point3(0.1,0,0.4);
    ray reflect_ray(this->rec.p, r);

    hit_record rec;

    if (world.hit(reflect_ray, 0.001, infinity, rec))
    {
        hit_node* refl_node = new hit_node();
        refl_node->parent = this;
        refl_node->r = reflect_ray;
        refl_node->rec = rec;

        //reflection rays
        for (int i = 0; i < refl_samples; i++)
        {
            hit_node* reflection = refl_node->get_reflection_ray(depth-1, refl_samples, refr_samples, world, refl_node->rec.material->roughness);
            if (reflection != nullptr)
            {
                refl_node->reflection_ray.push_back(reflection);
            }
        }
        //secondary transmission
        if (rec.material->is_transmissive())
        {
            for (int i = 0; i < refr_samples; i++)
            {
                hit_node* transmission = refl_node->get_transmission_ray(depth-1, refl_samples, refr_samples, world, refl_node->rec.material->refr_rough);
                if (transmission != nullptr)
                {
                    refl_node->transmission_ray.push_back(transmission);
                }
            }
        }
        return refl_node;
    }
    else
    {
        return nullptr;
    }
}

#endif