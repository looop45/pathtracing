#ifndef SAMPLER_H
#define SAMPLER_H

#include "utilities.hpp"
#include "scene.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include <stdio.h>
//#include "hit_tree.hpp"
#include "camera.hpp"
#include "hittable_list.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "light.hpp"

using namespace std;

class sampler
{
    public:
        //requires desired sample (power of 2) and pixel coordinates
        sampler(int samples, int pixel_x, int pixel_y, camera cam, shared_ptr<environment_color> bg)
        {
            this->pixel_x = pixel_x;
            this->pixel_y = pixel_y;
            this->samples = samples;
            this->cam = cam;
            this->bg = bg;
        }

        color sample_scene(hittable& world, hittable_list& lights, int depth);
        color ray_color(const ray& r, int depth, hittable& world, hittable_list& lights);

    private:

        int pixel_x;
        int pixel_y;
        int samples;

        shared_ptr<environment_color> bg;

        camera cam;

};

//casts random rays into scene for given pixel and sample count, returns averaged color
color sampler::sample_scene(hittable& world, hittable_list& lights, int depth)
{
    int sample_total = pow(samples, 2);
    float pixel_resolution =  1 / (float)samples;

    color color_total = color(0,0,0);

    //divide pixel by sample resolution, cast rays through jittered
    for (int i = 0; i < samples; i++)
    {
        for (int j = 0; j < samples; j++)
        {
            float x_jitter = random_double() * pixel_resolution/2;
            float y_jitter = random_double() * pixel_resolution/2;

            float x_coord = (i * pixel_resolution) + pixel_x + pixel_resolution/2 + x_jitter;
            float y_coord = (j * pixel_resolution) + pixel_y + pixel_resolution/2 + y_jitter;

            ray r = cam.getRay(x_coord, y_coord);

            //get color for ray
            color_total += ray_color(r, depth, world, lights);
        }
    }
    return clamp(color_total/sample_total);
}

color sampler::ray_color(const ray& r, int depth, hittable& world, hittable_list& lights)
        {
            //reached depth limit
            if (depth <= 0)
            {
                return color(0,0,0);
            }

            hit_record rec;
            scatter_record srec;
            color color_from_emission;
            
            if (world.hit(r, 0, 5, rec))
            {
                if (!rec.mat->scatter(r, rec, srec))
                {
                    color_from_emission = rec.mat->emitted(r, rec, rec.uv[0], rec.uv[1], rec.p);
                    return color_from_emission;
                }
                //recursively calculate path color

                //auto p0 = make_shared<hittable_pdf>(*lights.objects.at(0), rec.p);
                //auto p1 = make_shared<cosine_pdf>(rec.normal);
                //mixture_pdf mixed_pdf(p0, p1);

                //scattered = ray(rec.p, mixed_pdf.generate());
                //auto pdf_val = mixed_pdf.value(scattered.direction());
                
                color scatter_color = (srec.attenuation * srec.brdf * ray_color(srec.wi, depth-1, world, lights)) / srec.pdf_val;

                return scatter_color + color_from_emission;
            }

            //no hit, do gradient bg
            return bg->value(r.direction());
        }

#endif