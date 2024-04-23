#ifndef ONB_H
#define ONB_H

#include "utilities.hpp"

class onb {
  public:
    onb() {}

    vec3 operator[](int i) const { return axis[i]; }
    vec3& operator[](int i) { return axis[i]; }

    vec3 u() const { return axis[0]; }
    vec3 v() const { return axis[1]; }
    vec3 w() const { return axis[2]; }

    vec3 local_to_world(double a, double b, double c) const {
        return a*u() + b*v() + c*w();
    }

    vec3  local_to_world(const vec3& a) const {
        return a.x()*u() + a.y()*v() + a.z()*w();
    }

    vec3 world_to_local(const vec3& a) const
    {
      return a.x() * inv[0] + a.y()*inv[1] + a.z()*inv[2];
    }

    //
    void build_from_w(const vec3& w) {
        vec3 unit_w = unit_vector(w);
        vec3 a = (fabs(unit_w.x()) > 0.9) ? vec3(0,1,0) : vec3(1,0,0);
        vec3 v = unit_vector(cross(unit_w, a));
        vec3 u = cross(unit_w, v);
        axis[0] = u;
        axis[1] = v;
        axis[2] = unit_w;
        calculate_inverse();
    }

    void calculate_inverse()
    {
      for (int i = 0; i < 4; i++)
      {
        for (int j = 0; j < 4; j++)
        {
          inv[i][j] = axis[j][i];
        }
      }
    }

  public:
    vec3 axis[3];
    vec3 inv[3];
};


#endif