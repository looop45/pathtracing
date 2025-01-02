#ifndef MATRIX4_H
#define MATRIX4_H

#include "utilities.hpp"

#include <cmath>
#include <iostream>

using namespace std;

class matrix4
{
    public:

        matrix4()
        {
            r[0] = vec4();
            r[1] = vec4();
            r[2] = vec4();
            r[3] = vec4();
        }
        matrix4(vec4 r1, vec4 r2, vec4 r3, vec4 r4)
        {
            r[0] = r1;
            r[1] = r2;
            r[2] = r3;
            r[3] = r4;
        }
        matrix4(double r11, double r12, double r13, double r14,
                double r21, double r22, double r23, double r24,
                double r31, double r32, double r33, double r34,
                double r41, double r42, double r43, double r44)
        {
            r[0] = vec4(r11, r12, r13, r14);
            r[1] = vec4(r21, r22, r23, r24);
            r[2] = vec4(r31, r32, r33, r34);
            r[3] = vec4(r41, r42, r43, r44);
        }

        matrix4 operator-() const {return matrix4(-r[0], -r[1], -r[2], -r[3]);}
        vec4 operator[] (int i) const {return r[i];}
        vec4& operator[] (int i) {return r[i];}

        vec4 column(int i)
        {
            return vec4(r[0][i], r[1][i], r[2][i], r[3][i]);
        }

        matrix4 invert()
        {
            matrix4 inv;
            double det;
            int i;

            inv[0][0]  =  r[1][1] * r[2][2] * r[3][3] - r[1][1] * r[2][3] * r[3][2] - r[2][1] * r[1][2] * r[3][3] + r[2][1] * r[1][3] * r[3][2] + r[3][1] * r[1][2] * r[2][3] - r[3][1] * r[1][3] * r[2][2];
            inv[1][0]  = -r[0][4] * r[2][2] * r[3][3] + r[0][4] * r[2][3] * r[3][2] + r[2][0] * r[1][2] * r[3][3] - r[2][0] * r[1][3] * r[3][2] - r[3][0] * r[1][2] * r[2][3] + r[3][0] * r[1][3] * r[2][2];
            inv[2][0]  =  r[0][4] * r[2][1]  * r[3][3] - r[0][4] * r[2][3] * r[3][1] - r[2][0] * r[1][1] * r[3][3] + r[2][0] * r[1][3] * r[3][1] + r[3][0] * r[1][1] * r[2][3] - r[3][0] * r[1][3] * r[2][1];
            inv[3][0] = -r[0][4] * r[2][1]  * r[3][2] + r[0][4] * r[2][2] * r[3][1] + r[2][0] * r[1][1] * r[3][2] - r[2][0] * r[1][2] * r[3][1] - r[3][0] * r[1][1] * r[2][2] + r[3][0] * r[1][2] * r[2][1];
            inv[0][1]  = -r[0][1] * r[2][2] * r[3][3] + r[0][1] * r[2][3] * r[3][2] + r[2][1] * r[0][2] * r[3][3] - r[2][1] * r[0][3] * r[3][2] - r[3][1] * r[0][2] * r[2][3] + r[3][1] * r[0][3] * r[2][2];
            inv[1][1]  =  r[0][0] * r[2][2] * r[3][3] - r[0][0] * r[2][3] * r[3][2] - r[2][0] * r[0][2] * r[3][3] + r[2][0] * r[0][3] * r[3][2] + r[3][0] * r[0][2] * r[2][3] - r[3][0] * r[0][3] * r[2][2];
            inv[2][1]  = -r[0][0] * r[2][1]  * r[3][3] + r[0][0] * r[2][3] * r[3][1] + r[2][0] * r[0][1] * r[3][3] - r[2][0] * r[0][3] * r[3][1] - r[3][0] * r[0][1] * r[2][3] + r[3][0] * r[0][3] * r[2][1];
            inv[3][1] =  r[0][0] * r[2][1]  * r[3][2] - r[0][0] * r[2][2] * r[3][1] - r[2][0] * r[0][1] * r[3][2] + r[2][0] * r[0][2] * r[3][1] + r[3][0] * r[0][1] * r[2][2] - r[3][0] * r[0][2] * r[2][1];
            inv[0][2]  =  r[0][1] * r[1][2]  * r[3][3] - r[0][1] * r[1][3]  * r[3][2] - r[1][1] * r[0][2] * r[3][3] + r[1][1] * r[0][3] * r[3][2] + r[3][1] * r[0][2] * r[1][3]  - r[3][1] * r[0][3] * r[1][2];
            inv[1][2]  = -r[0][0] * r[1][2]  * r[3][3] + r[0][0] * r[1][3]  * r[3][2] + r[0][4] * r[0][2] * r[3][3] - r[0][4] * r[0][3] * r[3][2] - r[3][0] * r[0][2] * r[1][3]  + r[3][0] * r[0][3] * r[1][2];
            inv[2][2] =  r[0][0] * r[1][1]  * r[3][3] - r[0][0] * r[1][3]  * r[3][1] - r[0][4] * r[0][1] * r[3][3] + r[0][4] * r[0][3] * r[3][1] + r[3][0] * r[0][1] * r[1][3]  - r[3][0] * r[0][3] * r[1][1];
            inv[3][2] = -r[0][0] * r[1][1]  * r[3][2] + r[0][0] * r[1][2]  * r[3][1] + r[0][4] * r[0][1] * r[3][2] - r[0][4] * r[0][2] * r[3][1] - r[3][0] * r[0][1] * r[1][2]  + r[3][0] * r[0][2] * r[1][1];
            inv[0][3]  = -r[0][1] * r[1][2]  * r[2][3] + r[0][1] * r[1][3]  * r[2][2] + r[1][1] * r[0][2] * r[2][3] - r[1][1] * r[0][3] * r[2][2] - r[2][1]  * r[0][2] * r[1][3]  + r[2][1]  * r[0][3] * r[1][2];
            inv[1][3]  =  r[0][0] * r[1][2]  * r[2][3] - r[0][0] * r[1][3]  * r[2][2] - r[0][4] * r[0][2] * r[2][3] + r[0][4] * r[0][3] * r[2][2] + r[2][0]  * r[0][2] * r[1][3]  - r[2][0]  * r[0][3] * r[1][2];
            inv[2][3] = -r[0][0] * r[1][1]  * r[2][3] + r[0][0] * r[1][3]  * r[2][1]  + r[0][4] * r[0][1] * r[2][3] - r[0][4] * r[0][3] * r[2][1]  - r[2][0]  * r[0][1] * r[1][3]  + r[2][0]  * r[0][3] * r[1][1];
            inv[3][3] =  r[0][0] * r[1][1]  * r[2][2] - r[0][0] * r[1][2]  * r[2][1]  - r[0][4] * r[0][1] * r[2][2] + r[0][4] * r[0][2] * r[2][1]  + r[2][0]  * r[0][1] * r[1][2]  - r[2][0]  * r[0][2] * r[1][1];

            det = r[0][0] * inv[0][0] + r[0][1] * inv[1][0] + r[0][2] * inv[2][0] + r[0][3] * inv[3][0];
            if (det == 0) return matrix4();

            det = 1.0/det;

            for (i = 0; i < 4; i++)
            {
                for (j = 0; j < 4; j++)
                {
                    inv[i][j] = inv[i][j] * det;
                }
            }

            return inv;

        }

    public:
        vec4 r[4];

};

//multiplying matrix4 by vec4
inline vec4 operator*(const matrix4 &u, const vec4 &v)
{
    vec4 r;
    r[0] = sumv4(v * u.r[0]);
    r[1] = sumv4(v * u.r[1]);
    r[2] = sumv4(v * u.r[2]);
    r[3] = sumv4(v * u.r[3]);
    return r;
}

//multiplying matrix4 by vec3... Yeah I know. It's bad, I just drop the 4th value. Sue me.
inline vec3 operator*(const matrix4 &u, const vec3 &v)
{
    vec3 r;
    r[0] = sumv3(v * u.r[0]);
    r[1] = sumv3(v * u.r[1]);
    r[2] = sumv3(v * u.r[2]);
    return r;
}

//multiplying 2 matrices together
inline matrix4 operator*(matrix4 &u, matrix4 &v)
{
    matrix4 m;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = sumv4(u.r[i] * v.column(j));
        }
    }
    return m;
}


#endif