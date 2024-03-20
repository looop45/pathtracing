#ifndef MESH_H
#define MESH_H

#include "vec3.hpp"
#include "vec2.hpp"
#include "ray.hpp"
#include <math.h>
#include "shadingModel.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "polygon.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

class mesh : public hittable_list
{
    public:
        mesh(const char* path, point3 pos, double scale, shadingModel* material)
        {
            this->path = path;
            this->pos = pos;
            this->scale = scale;
            this->material = material;
            this->objects = this->read_file();
        }

    private:
        const char* path;
        point3 pos;
        double scale;
        shadingModel* material;

        vector<shared_ptr<hittable>> read_file();


};

vector<shared_ptr<hittable>> mesh::read_file()
{
    vector<vec3> temp_points;
    vector<vec2> uvs;
    vector<vec3> normals;

    vector<shared_ptr<hittable>> faces;

    string word;

    ifstream file(path);

    if (file.is_open())
    {
        while(file >> word)
        {
            if ( strcmp(word.c_str(), "v") == 0 )
            {
                double x, y, z;
                file >> x >> y >> z;
                temp_points.push_back(point3(x,y,z));
            }else if ( strcmp( word.c_str(), "vt" ) == 0 )
            {
                double u, v;
                file >> u >> v;
                uvs.push_back(vec2(u,v));
            }else if ( strcmp( word.c_str(), "vn" ) == 0 )
            {
                double x, y, z;
                file >> x >> y >> z;
                normals.push_back(vec3(x,y,z));
            }else if ( strcmp( word.c_str(), "f" ) == 0 )
            {
                int vertexIndex[3], uvIndex[3], normalIndex[3];

                string v1, v2, v3;

                file >> v1;
                stringstream v1_ss(v1);
                file >> v2;
                stringstream v2_ss(v2);
                file >> v3;
                stringstream v3_ss(v3);


                string token;
                getline(v1_ss, token, '/');
                vertexIndex[0] = stoi(token);
                getline(v1_ss, token, '/');
                uvIndex[0] = stoi(token);
                getline(v1_ss, token, '/');
                normalIndex[0] = stoi(token);

                getline(v2_ss, token, '/');
                vertexIndex[1] = stoi(token);
                getline(v2_ss, token, '/');
                uvIndex[1] = stoi(token);
                getline(v2_ss, token, '/');
                normalIndex[1] = stoi(token);

                getline(v3_ss, token, '/');
                vertexIndex[2] = stoi(token);
                getline(v3_ss, token, '/');
                uvIndex[2] = stoi(token);
                getline(v3_ss, token, '/');
                normalIndex[2] = stoi(token);


                vector<shared_ptr<vertex>> face_verts;
                for (int i = 0; i < 3; i++)
                {
                    face_verts.push_back(make_shared<vertex>(temp_points.at(vertexIndex[i]-1) * scale + pos, uvs.at(uvIndex[i]-1), normals.at(normalIndex[i]-1)));
                }

                faces.push_back(make_shared<polygon>(face_verts, this->material));

            }
        }
        file.close();
    }
    
    return faces;
}
#endif