#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;
using namespace std;

class vec3
{
    public:
        //CONSTRUCTORS
        vec3() 
        {
            o[0] = 0.0;
            o[1] = 0.0;
            o[2] = 0.0;}
        vec3(double o1, double o2, double o3)
        {
            o[0] = o1;
            o[1] = o2;
            o[2] = o3;
        }

        //GETTERS
        double x() const {return o[0];}
        double y() const {return o[1];}
        double z() const {return o[2];}

        //OPERATOR MAGIC - the walls told me to do it...
        vec3 operator-() const {return vec3(-o[0], -o[1], -o[2]);}
        double operator[] (int i) const {return o[i];}
        double& operator[] (int i) {return o[i];}

        vec3& operator+=(const vec3 &v) 
        {
            o[0] += v.o[0];
            o[1] += v.o[1];
            o[2] += v.o[2];
            return *this;
        }

        vec3& operator*=(const double t) 
        {
            o[0] *= t;
            o[1] *= t;
            o[2] *= t;
            return *this;
        }

        vec3& operator/=(const double t)
        {
            return *this *= 1/t; //hehe tricky tricky
        }

        double length() const
        {
            return sqrt(length_squared());
        }

        double length_squared() const
        {
            return o[0] * o[0] + o[1] * o[1] + o[2] * o[2];
        }


    public:
        //VALUES
        double o[3];
};

//Aliases, Secret Identities, AKA
using point3 = vec3;
using color = vec3;

//OTHER UTILITIES
inline ostream& operator<<(ostream &out, const vec3 &v)
{
    return out << v.o[0] << ' ' << v.o[1] << ' ' << v.o[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) 
{
    return vec3(u.o[0] + v.o[0], u.o[1] + v.o[1], u.o[2] + v.o[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) 
{
    return vec3(u.o[0] - v.o[0], u.o[1] - v.o[1], u.o[2] - v.o[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) 
{
    return vec3(u.o[0] * v.o[0], u.o[1] * v.o[1], u.o[2] * v.o[2]);
}

inline vec3 operator/(const vec3 &u, const vec3 &v) 
{
    return vec3(u.o[0] / v.o[0], u.o[1] / v.o[1], u.o[2] / v.o[2]);
}

inline vec3 operator*(double t, const vec3 &v) 
{
    return vec3(t*v.o[0], t*v.o[1], t*v.o[2]);
}

inline vec3 operator*(const vec3 &v, double t) 
{
    return t * v;
}

inline vec3 operator/(vec3 v, double t) 
{
    return (1/t) * v;
}

inline bool operator<=(const point3& v1, const point3& v2)
{
    if (v1.o[0] <= v2.o[0] && v1.o[1] <= v2.o[1] && v1.o[2] <= v2.o[2])
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline bool operator>=(const vec3 v1, const vec3 v2)
{
    if (v1.o[0] >= v2.o[0] && v1.o[1] >= v2.o[1] && v1.o[2] >= v2.o[2])
    {
        return true;
    }
    else
    {
        return false;
    } 
}

inline double dot(const vec3 &u, const vec3 &v) 
{
    return u.o[0] * v.o[0]
        + u.o[1] * v.o[1]
        + u.o[2] * v.o[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) 
{
    return vec3(u.o[1] * v.o[2] - u.o[2] * v.o[1],
                u.o[2] * v.o[0] - u.o[0] * v.o[2],
                u.o[0] * v.o[1] - u.o[1] * v.o[0]);
}

inline vec3 unit_vector(vec3 v) 
{
    return v / v.length();
}

inline double maximize(vec3 v) 
{
    if (abs(v.x()) > abs(v.y()) && abs(v.x()) > abs(v.z()))
    {
        return v.x();
    }
    else if (abs(v.y()) > abs(v.x()) && abs(v.y()) > abs(v.z()))
    {
        return v.y();
    }
    else
    {
        return v.z();
    }
}

inline vec3 epsilon(vec3 v)
{
    double offX = ((double) rand() / (RAND_MAX));
    double offY = ((double) rand() / (RAND_MAX));
    double offZ = ((double) rand() / (RAND_MAX));

    vec3 out(v.x() + offX, v.y() + offY, v.z() + offZ);

    return out;
}

#endif