#ifndef RAY_H
#define RAY_H

#include "vec3.hpp"
#include "utilities.hpp"

class ray
{
    public:

        //CONSTRUCTORS
        ray() {}
        ray(point3& origin, vec3& direction)
        {
            Ro = origin;
            Rd = direction;
        }

        ray(const point3& origin, const vec3& direction)
        {
            Ro = origin;
            Rd = direction;
        }

        //GETTERS
        point3 origin() const { return Ro; }
        vec3 direction() const {return Rd; }

        point3 at(float t) const
        {
            return Ro + t * Rd;
        }

    private:
        point3 Ro;
        vec3 Rd;

};
#endif

