#ifndef VEC2_H
#define VEC2_H

#include <cmath>
#include <iostream>

using std::sqrt;
using namespace std;

class vec2
{
    public:
        //CONSTRUCTORS
        vec2() 
        {
            o[0] = 0.0;
            o[1] = 0.0;
        }
        vec2(double o1, double o2)
        {
            o[0] = o1;
            o[1] = o2;
        }

        //GETTERS
        double x() const {return o[0];}
        double y() const {return o[1];}

        double u() const {return o[0];}
        double v() const {return o[1];}

        //OPERATOR MAGIC - the walls told me to do it...
        vec2 operator-() const {return vec2(-o[0], -o[1]);}
        double operator[] (int i) const {return o[i];}
        double& operator[] (int i) {return o[i];}

        vec2& operator+=(const vec2 &v) 
        {
            o[0] += v.o[0];
            o[1] += v.o[1];
            return *this;
        }

        vec2& operator*=(const double t) 
        {
            o[0] *= t;
            o[1] *= t;
            return *this;
        }

        vec2& operator/=(const double t)
        {
            return *this *= 1/t; //hehe tricky tricky
        }

        double length() const
        {
            return sqrt(length_squared());
        }

        double length_squared() const
        {
            return o[0] * o[0] + o[1] * o[1];
        }


    public:
        //VALUES
        double o[2];
};

//Aliases, Secret Identities, AKA
using point2 = vec2;
using uv = vec2;

//OTHER UTILITIES
inline ostream& operator<<(ostream &out, const vec2 &v)
{
    return out << v.o[0] << ' ' << v.o[1];
}

inline vec2 operator+(const vec2 &u, const vec2 &v) 
{
    return vec2(u.o[0] + v.o[0], u.o[1] + v.o[1]);
}

inline vec2 operator-(const vec2 &u, const vec2 &v) 
{
    return vec2(u.o[0] - v.o[0], u.o[1] - v.o[1]);
}

inline vec2 operator*(const vec2 &u, const vec2 &v) 
{
    return vec2(u.o[0] * v.o[0], u.o[1] * v.o[1]);
}

inline vec2 operator*(double t, const vec2 &v) 
{
    return vec2(t*v.o[0], t*v.o[1]);
}

inline vec2 operator*(const vec2 &v, double t) 
{
    return t * v;
}

inline vec2 operator/(vec2 v, double t) 
{
    return (1/t) * v;
}

inline double dot(const vec2 &u, const vec2 &v) 
{
    return u.o[0] * v.o[0]
        + u.o[1] * v.o[1];
}

inline vec2 cross(const vec2 &u, const vec2 &v) 
{
    return vec2(u.o[1] * v.o[2] - u.o[2] * v.o[1],
                u.o[2] * v.o[0] - u.o[0] * v.o[2]);
}

inline vec2 unit_vector(vec2 v) 
{
    return v / v.length();
}

inline double maximize(vec2 v) 
{
    if (v.x() > v.y())
    {
        return v.x();
    }
    else
    {
        return v.y();
    }
}

#endif