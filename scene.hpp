#ifndef SCENE_H
#define SCENE_H
 
#include "utilities.hpp"
#include <vector>
#include "camera.hpp"
#include "hittable_list.hpp"
#include "sampler.hpp"
#include "bvh_tree.hpp"
#include "texture.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>

using namespace std;

const int SAMPLES = 5;
const int REFL_SAMPLES = 2;
const int REFR_SAMPLES = 4;
const int RAY_DEPTH = 8;

class scene
{
    public:
        //Scene constructor
        scene(camera cam, shared_ptr<environment_color> bgColor, hittable_list surfaces, hittable_list lights, int samples, int width, int height)
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
                for (int i = 0; i < WIDTH; i++)
                {
                    //sampler: structure for generating samples for a given pixel.
                    sampler Sampler = sampler(SAMPLES, i, j, cam, bgColor);
                    color pixel_color = Sampler.sample_scene(surfaces, lights, RAY_DEPTH); //given a list of surfaces, returns a color for the given pixel that the ray intersects
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
        shared_ptr<environment_color> bgColor;

        hittable_list surfaces; //all the surfaces in a scene that a ray can hit
        hittable_list lights;

};



#endif