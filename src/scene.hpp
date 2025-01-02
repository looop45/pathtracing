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

const int SAMPLES = 60;
const int REFL_SAMPLES = 2;
const int REFR_SAMPLES = 1;
const int RAY_DEPTH = 3;

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

            vector<vector<color>> pixel_colors(HEIGHT, vector<color>(WIDTH, color(0,0,0)));

            unsigned int num_threads = thread::hardware_concurrency();

            auto renderStrip = [&](int start_y, int end_y)
            {
                for (int j = start_y; j < end_y; j++)
                {
                    for (int i = 0; i < WIDTH; i++)
                    {
                        sampler Sampler(SAMPLES, i, j, cam, bgColor);
                        pixel_colors[j][i] = Sampler.sample_scene(surfaces, lights, RAY_DEPTH);
                    }
                }
            };

            int rows_per_thread = HEIGHT / num_threads;

            vector<thread> threads;

            for (unsigned int t = 0; t < num_threads; ++t)
            {
                int start_y = t * rows_per_thread;
                int end_y = (t == num_threads - 1) ? HEIGHT : (t+1) * rows_per_thread;
                threads.emplace_back(renderStrip, start_y, end_y);
            }

            for (auto &t : threads)
            {
                t.join();
            }

            //write out file
            ofstream outFile;
            outFile.open(name + ".ppm");

            outFile << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";
            #pragma omp parallel for collapse(2) schedule(dynamic)
            for (int j = HEIGHT - 1; j >= 0; j--)
            {
                for (int i = 0; i < WIDTH; i++)
                {
                   write_color(outFile, pixel_colors[j][i]);
                }
            }
            outFile.close();

            cout << "\nBEEP BOOP BEEP BOOP DONE!" << endl;
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