#ifndef SAMPLER_H
#define SAMPLER_H

#include<random>
#include<iostream>
#include<memory>
#include <vector>
#include<stdio.h>
#include "hit_tree.hpp"
#include "camera.hpp"
#include "hittable_list.hpp"
#include "hittable.hpp"

using namespace std;


class sampler
{
    public:
        //requires desired sample (power of 2) and pixel coordinates
        sampler(int samples, int refl_samples, int refr_samples, int pixel_x, int pixel_y, camera cam, color bgColor)
        {
            this->pixel_x = pixel_x;
            this->pixel_y = pixel_y;
            this->samples = samples;
            this->refl_samples = refl_samples;
            this->refr_samples = refr_samples;
            this->cam = cam;
            this->bgColor = bgColor;
        }

        color sample_scene(hittable& world, vector<shared_ptr<light>> lights);

    private:

        int pixel_x;
        int pixel_y;
        int samples;
        int refl_samples;
        int refr_samples;

        color bgColor;

        camera cam;

        color ray_color(const ray& r, hittable& world, vector<shared_ptr<light>> lights);
};

//casts random rays into scene for given pixel and sample count, returns averaged color
color sampler::sample_scene(hittable& world, vector<shared_ptr<light>> lights)
{


    int sample_total = pow(samples, 2);
    float pixel_resolution =  1 / (float)samples;

    color color_total = color(0,0,0);

    //divide pixel by sample resolution, cast rays through jittered
    for (int i = 0; i < samples; i++)
    {
        for (int j = 0; j < samples; j++)
        {
            float x_jitter = distribution(gen) * pixel_resolution/2;
            float y_jitter = distribution(gen) * pixel_resolution/2;

            float x_coord = (i * pixel_resolution) + pixel_x + pixel_resolution/2 + x_jitter;
            float y_coord = (j * pixel_resolution) + pixel_y + pixel_resolution/2 + y_jitter;

            ray r = cam.getRay(x_coord, y_coord);

            //get color for ray
            color_total += ray_color(r, world, lights);
        }
    }
    return color_total/sample_total;
}

color sampler::ray_color(const ray& r, hittable& world, vector<shared_ptr<light>> lights)
        {
            hit_record rec;
            if (world.hit(r, 0, infinity, rec))
            {
                //default color
                color out = color(0,0,0);

                //
                hit_tree tree(r, rec, world, lights, bgColor, refl_samples, refr_samples);
                out = tree.evaluate();

                return out;
            }
            //no hit, do gradient bg
            return bgColor;
        }

#endif