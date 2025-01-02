#ifndef VEC4_H
#define VEC4_H

#include "utilities.hpp"
#include <cmath>
#include <iostream>

using std::sqrt;
using namespace std;

class vec4
{
    public:
        //CONSTRUCTORS
        vec4() 
        {
            o[0] = 0.0;
            o[1] = 0.0;
            o[2] = 0.0;
            o[3] = 0.0;
        }
        vec4(double o1, double o2, double o3, double o4)
        {
            o[0] = o1;
            o[1] = o2;
            o[2] = o3;
            o[3] = o4;
        }

        //GETTERS
        double x() const {return o[0];}
        double y() const {return o[1];}
        double z() const {return o[2];}
        double w() const {return o[3];}

        //OPERATOR MAGIC - the walls told me to do it...
        vec4 operator-() const {return vec4(-o[0], -o[1], -o[2], -o[3]);}
        double operator[] (int i) const {return o[i];}
        double& operator[] (int i) {return o[i];}

        vec4& operator+=(const vec4 &v) 
        {
            o[0] += v.o[0];
            o[1] += v.o[1];
            o[2] += v.o[2];
            o[3] += v.o[3];
            return *this;
        }

        vec4& operator*=(const double t) 
        {
            o[0] *= t;
            o[1] *= t;
            o[2] *= t;
            o[3] *= t;
            return *this;
        }

        vec4& operator/=(const double t)
        {
            return *this *= 1/t; //hehe tricky tricky
        }

        double length() const
        {
            return sqrt(length_squared());
        }

        double length_squared() const
        {
            return o[0] * o[0] + o[1] * o[1] + o[2] * o[2] + o[3] * o[3];
        }


    public:
        //VALUES
        double o[4];
};

//Aliases, Secret Identities, AKA
using point4 = vec4;


#endif