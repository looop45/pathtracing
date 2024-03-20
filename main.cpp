#include <iostream>
#include <fstream>

#include "vec3.hpp"
#include "color.hpp"
#include "camera.hpp"
#include "sphere.hpp"
#include "polygon.hpp" 
#include "shadingModel.hpp"
#include "scene.hpp"
#include "distant_light.hpp"
#include "point_light.hpp"
#include "area_light.hpp"
#include "mesh.hpp"
#include "vertex.hpp"
#include "image_texture.hpp"
#include <string>
#include <chrono>


using namespace std;

int main()
{
    const int WIDTH = 512;
    const int HEIGHT = 512;

    //camera
    point3 camera_org(0,0,1);
    vec3 camera_dir(0,0,1);
    float fov = 150.0f;

    color lightColor(0,1,1);

    camera cam(camera_org, camera_dir, fov, WIDTH, HEIGHT);

    color bgColor = color(0.2,0.2,0.2);


    hittable_list objects;
    vec3 lightDir = vec3(-1,-1,-1);

    vector<shared_ptr<light>> lights;
    color ambientColor(1, 1, 1); 

    //shaders
    shadingModel translucent(0, 1, 0.01, color
    (1, 1, 1), ambientColor, color(1.0, 1.0, 1.0), 128, 0, 0, 1, 1.05,0.02);

    shadingModel red_Sphere(0.8, 0.2, 0.25, color(1.0, 0, 0), ambientColor, color(1.0, 1.0, 1.0), 6, 0, 0, 0, 0,0);
    shadingModel blue_Sphere(0.8, 0.2, 0.25, color(0, 0, 1.0), ambientColor, color(1.0, 1.0, 1.0), 6, 0, 0, 0, 0,0);
    shadingModel refl_Sphere(0, 1, 0.15, color(1.0, 1.0, 1.0), ambientColor, color(1.0, 1.0, 1.0), 64, 1, 0.05, 0, 0,0);
    shadingModel mat0(0.7, 1.0, 0.1, color(1,1,1), ambientColor, color(1.0,1.0,1.0), 4, 0.5, 0.1, 0, 0,0);
    
    image_texture image("egbert.jpeg");
    image_texture marb_img("marble.jpg");

    shadingModel parris(0.8, 0.2, 0.15, &image, ambientColor, color(1.0, 1.0, 1.0), 64, 0.1 , 0, 0, 0,0);
    shadingModel marble(0.8, 0.8, 0.15, &marb_img, ambientColor, color(1.0, 1.0, 1.0), 64, 0 , 0, 0, 0,0);

    point3 translate(0,-0.25,-1);

    /*for (int i = 0; i < 10; i++)
    {
        double x = sin(i * (2*3.14)/10);
        double y = cos(i * (2*3.14)/10);
        double z = tan(i * (2*3.14)/10);
        double r = sin(i * (2*3.14)/10) * 0.5 + 0.5;
        double g = sin((i+3.333) * (2*3.14)/10) * 0.5 + 0.5;
        double b = sin((i+6.666) * (2*3.14)/10) * 0.5 + 0.5;
        shadingModel thing(0.8, 0.5, 0.25, color(r, g, b), ambientColor, color(1,1,1), 128, 0.5, 0.1, 0, 0, 0);
        objects.add(make_shared<sphere>(point3(x * 0.5, y * 0.5, -1) , 0.1, thing));
    }*/

    /*objects.add(make_shared<quad>(point3(-1, -1, 0), vec3(2,0,0), vec3(0,0.1,-2), &marble));
    objects.add(make_shared<quad>(point3(-1, -1, 0), vec3(0,0,-2), vec3(0,2,0), &marble));
    objects.add(make_shared<quad>(point3(-1, -1, -1), vec3(2,0,0), vec3(0,2,0), &marble));
    objects.add(make_shared<quad>(point3(-1, 1, 0), vec3(2,0,0), vec3(0,0,-2), &marble));
    objects.add(make_shared<quad>(point3(1, -1, 0), vec3(0,0,-2), vec3(0,2,0), &marble));
*/
    
    objects.add(mesh("suztex.obj", point3(0, -0.5, -2), 1, &parris));
    //objects.add(mesh("plane.obj", point3(0,0,-2), 1, &parris));
    //objects.add(make_shared<sphere>(point3(-0.5, 0, -1), 0.3, &parris));
    //objects.add(make_shared<sphere>(point3(0, 1, -1), 0.3, &parris));
    //objects.add(make_shared<sphere>(point3(0, -1, -1), 0.3, &parris));

    //objects.add(make_shared<sphere>(point3(0.5, 0, -1), 0.3, &parris));
    //objects.add(make_shared<sphere>(point3(-0.5,0,0) + translate, 0.2, translucent));
    //objects.add(make_shared<sphere>(point3(-0.5,0,-1) + translate, 0.2, blue_Sphere));



    //objects.add(make_shared<sphere>(point3(0,0.25,0) + translate, 0.2, refl_Sphere));
    //objects.add(make_shared<sphere>(point3(0.5,0,0) + translate, 0.2, red_Sphere));
    //objects.add(make_shared<bounding_volume>(point3(-1, -0.2, -1) + translate, point3(-0.6,0.2,-0.1) + translate, white_Sphere));

    //objects.add(make_shared<sphere>(point3(0.0, -2, -2), 1.8, white_Sphere));

    //planes
    vector<shared_ptr<vertex>> points;
    points.push_back(make_shared<vertex>(-2,-1,-1));
    points.push_back(make_shared<vertex>(2,-1,-1));
    points.push_back(make_shared<vertex>(-2,1,-1));

    //objects.add(make_shared<polygon>(points, &mat0));

    points.clear();
    points.push_back(make_shared<vertex>(-2,-0.75,-5));
    points.push_back(make_shared<vertex>(2,-0.75,0));
    points.push_back(make_shared<vertex>(2,-0.75,-5));

    //objects.add(make_shared<polygon>(points, &mat0));


    lights.push_back(make_shared<point_light>(point3(-1,1,1), color(1,1,0.6), 1.5));
    //lights.push_back(make_shared<distant_light>(lightDir, color(1,1,1), 0.6));
    lights.push_back(make_shared<area_light>(point3(-0.4, 1, 0), vec3(1,0,0), vec3(0,0,-1), color(1,1,1), 2, 1));
    //objects.add(make_shared<sphere>(point3(spherex,0.25, spherey), 0.2, refl_Sphere));

    scene scene1 = scene(cam, bgColor, objects, lights, 8, WIDTH, HEIGHT); 
    auto beginTime = chrono::high_resolution_clock::now();
    scene1.traceScene("scene");
    auto endTime = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::seconds>(endTime - beginTime).count();
    cout << "Execution time: " << duration << " sec" << endl;

    lights.clear();
    objects.clear();

    return 0;

}