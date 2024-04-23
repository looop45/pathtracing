#include <iostream>
#include <fstream>

#include "utilities.hpp"
#include "camera.hpp"
#include "sphere.hpp"
//#include "polygon.hpp" 
#include "material.hpp"
#include "scene.hpp"
#include "distant_light.hpp"
#include "material.hpp"
#include "point_light.hpp"
//#include "area_light.hpp"
#include "mesh.hpp"
//#include "vertex.hpp"
#include "texture.hpp"
#include "quad.hpp"
#include <string>
#include <chrono>


using namespace std;

int main()
{
    const int WIDTH = 100;
    const int HEIGHT = 100;

    //camera
    point3 camera_org(0,0,1);
    vec3 camera_dir(0,0,1);
    float fov = 150.0f;

    color bgcol(1,1,1);

    camera cam(camera_org, camera_dir, fov, WIDTH, HEIGHT);
    shared_ptr<environment_color> env = make_shared<environment_color>("rest.jpeg");
    shared_ptr<environment_color> black_env = make_shared<environment_color>(color(0,0,0));

    hittable_list objects;
    hittable_list lights;

    //make colors
    shared_ptr<solid_color> whitecol = make_shared<solid_color>(1,1,1);
    shared_ptr<solid_color> redcol = make_shared<solid_color>(1,0,0);
    shared_ptr<solid_color> greencol = make_shared<solid_color>(0,1,0);


    //materials
    shared_ptr<lambertian> white = make_shared<lambertian>(whitecol);
    shared_ptr<lambertian> red = make_shared<lambertian>(redcol);
    shared_ptr<lambertian> green = make_shared<lambertian>(greencol);

    shared_ptr<emissive> emit = make_shared<emissive>(bgcol, 30);

    //add objects
    //objects.add(make_shared<sphere>(point3(0,0,-0.5), 0.5, white));
    //objects.add(make_shared<quad>(point3(-0.25, 0.5, 1), vec3(0.5,0,0), vec3(0,0.34,-0.5), emit));
    //objects.add(make_shared<quad>(point3(-2, -0.5, 0.25), vec3(4,0,0), vec3(0,0,-2.25), red));

    //cornell
    vec3 transform(0,-1,-1);
    float size = 1;
    shared_ptr<quad> light = make_shared<quad>(point3(-0.25, 0.99, -0.5), vec3(0.5,0,0), vec3(0,0,-0.5), emit);
    objects.add(light);
    lights.add(light);
    //lights.add()
    objects.add(make_shared<mesh>("c_white.obj", transform, size, white));
    objects.add(make_shared<mesh>("c_green.obj", transform, size, green));
    objects.add(make_shared<mesh>("c_red.obj", transform, size, red));



    scene scene1 = scene(cam, black_env, objects, lights, 8, WIDTH, HEIGHT);

    auto beginTime = chrono::high_resolution_clock::now();
    scene1.traceScene("scene");
    auto endTime = chrono::high_resolution_clock::now();

    string ad;
    //cin >> ad;

    auto duration = chrono::duration_cast<chrono::seconds>(endTime - beginTime).count();
    cout << "Execution time: " << duration << " sec" << endl;

    lights.clear();
    objects.clear();

    return 0;

}