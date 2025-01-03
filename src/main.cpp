#include <iostream>
#include <fstream>

#include "utilities.hpp"
#include "camera.hpp"
#include "sphere.hpp"
#include "material.hpp"
#include "scene.hpp"
#include "distant_light.hpp"
#include "material.hpp"
#include "point_light.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include "quad.hpp"
#include <string>
#include <chrono>


using namespace std;

int main()
{
    ///////////////This is an example scene for demonstration//////////////////

    const int WIDTH = 512;
    const int HEIGHT = 512;

    //camera
    point3 camera_org(0,0,1);
    vec3 camera_dir(0,0,1);
    float fov = 150.0f;

    color bgcol(1,1,1);

    camera cam(camera_org, camera_dir, fov, WIDTH, HEIGHT);
    shared_ptr<environment_color> env = make_shared<environment_color>("../resources/rest.jpeg");

    //scene lists
    hittable_list objects;
    hittable_list lights;

    //make colors
    shared_ptr<solid_color> whitecol = make_shared<solid_color>(1,1,1);
    shared_ptr<solid_color> blackcol = make_shared<solid_color>(0.001,0.001,0.001);
    shared_ptr<solid_color> point5col = make_shared<solid_color>(0.05, 0.5, 0.5);
    shared_ptr<solid_color> point1col = make_shared<solid_color>(0.1, 0.1,0.1);


    shared_ptr<solid_color> redcol = make_shared<solid_color>(1,0,0);
    shared_ptr<solid_color> greencol = make_shared<solid_color>(0,1,0);


    //materials
    shared_ptr<lambertian> white = make_shared<lambertian>(whitecol);
    shared_ptr<lambertian> red = make_shared<lambertian>(redcol);
    shared_ptr<lambertian> green = make_shared<lambertian>(greencol);
    shared_ptr<glossy> spec = make_shared<glossy>(point5col, whitecol, blackcol);
    shared_ptr<glass> glassy = make_shared<glass>(point5col, 1.3);
    shared_ptr<glossy> redmet = make_shared<glossy>(point1col, redcol, whitecol);


    shared_ptr<emissive> emit = make_shared<emissive>(bgcol, 50);


    //cornell
    vec3 transform(0,-1,-1);
    float size = 1;
    shared_ptr<quad> light = make_shared<quad>(point3(-0.25, 0.99, -0.5), vec3(0.5,0,0), vec3(0,0,-0.5), emit);
    objects.add(light);

    objects.add(make_shared<mesh>("../resources/c_white.obj", transform, size, spec));
    objects.add(make_shared<mesh>("../resources/c_green.obj", transform, size, green));
    objects.add(make_shared<mesh>("../resources/c_red.obj", transform, size, red));

    scene scene1 = scene(cam, env, objects, lights, 0, WIDTH, HEIGHT);

    cout << "Tracing Scene!" << endl;

    auto beginTime = chrono::high_resolution_clock::now();
    scene1.traceScene("../out");
    auto endTime = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::seconds>(endTime - beginTime).count();
    cout << "Execution time: " << duration << " sec" << endl;

    lights.clear();
    objects.clear();

    return 0;

}