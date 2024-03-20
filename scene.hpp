#ifndef SCENE_H
#define SCENE_H

#include "vec3.hpp"
#include <vector>
#include "sphere.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "sampler.hpp"
#include "bvh_tree.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <memory>
#include <vector>

using namespace std;

const int SAMPLES = 1;
const int REFL_SAMPLES = 2;
const int REFR_SAMPLES = 4;

class scene
{
    public:
        //Scene constructor
        scene(camera cam, color bgColor, hittable_list surfaces, vector<shared_ptr<light>> lights, int samples, int width, int height)
        {
            this->cam = cam;
            this->bgColor = bgColor;
            this->surfaces = surfaces;
            this->lights = lights;
            this->WIDTH = width;
            this->HEIGHT = height;
            
        }

        void traceScene(string name)
        {
            //create BVH tree   
            bvh_tree bvtree(2, surfaces, 1);

            //write out file
            ofstream outFile;
            outFile.open(name + ".ppm");

            outFile << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";

            for (int j = HEIGHT - 1; j >= 0; j--)
            {
                //cout << "\rSCANLINES REMAINING: " << j << ' ' << flush;
                for (int i = 0; i < WIDTH; i++)
                {
                    //sampler: structure for generating samples for a given pixel.
                    sampler Sampler = sampler(SAMPLES, REFL_SAMPLES, REFR_SAMPLES, i, j, cam, bgColor);
                    color pixel_color = Sampler.sample_scene(surfaces, lights); //given a list of surfaces, returns a color for the given pixel that the ray intersects
                    write_color(outFile, pixel_color); //writes the color to the ppm file
                }
            }

            cout << "\nBEEP BOOP BEEP BOOP DONE!" << endl;

            outFile.close();
        }

    private:
        int WIDTH;
        int HEIGHT;

        camera cam;

        int samples;

        color ambientColor;
        color bgColor;

        hittable_list surfaces; //all the surfaces in a scene that a ray can hit
        vector<shared_ptr<light>> lights;

};

#endif