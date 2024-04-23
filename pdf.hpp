#ifndef PDF_H
#define PDF_H

#include "utilities.hpp"
#include "hittable_list.hpp"

#include "onb.hpp"

class pdf {
  public:
    virtual ~pdf() {}

    virtual double value(const vec3& wi) const = 0;
    virtual vec3 generate() = 0;
};

class cosine_pdf : public pdf 
{
  public:
     cosine_pdf(const vec3& w) { uvw.build_from_w(w); }

    //calculates pdf scattering value
    double value(const vec3& wi) const override {
        auto cosine_theta = dot(unit_vector(wi), uvw.w());
        return fmax(0, cosine_theta/pi);
    }

    //generates ray in random cosine weighted hemisphere on point normal.
    vec3 generate() override {
        return uvw.local_to_world(random_cosine_direction());
    }

  private:
    onb uvw;
};

class ggx_pdf : public pdf {
  public:
    ggx_pdf(vec3& wo, double roughness)
    {
      this->wo = wo;
      a2 = roughness * roughness;
    }

    //D term
    const double D()
    {
      double denom_sqrt = ((wgdotwm * wgdotwm) * (a2-1) + 1);
      D_val = a2 / abs(pi * denom_sqrt * denom_sqrt);
      return D_val;
    }

    //pdf of ggx given importance sampling of NDF
    double value(const vec3 &wi) const override 
    {
      return (D_val * wgdotwm)/(4 * dot(wo, wm));
    }

    //sampling direction of NDF for new microfacet normal (half vector)
    vec3 generate() override
    {
      double e0 = random_double();
      double e1 = random_double();

      auto theta = acos(sqrt((1 - e0) / ((a2 - 1) * e0 + 1)));
      auto phi = pi * pi * e1;

      //Spherical to cartesian coordinates
      wm = spherical_to_cartesian(theta, phi);
      wgdotwm = dot(wg, wm);

      //convert to cartesian
      return spherical_to_cartesian(theta, phi);
    }

  private:
    const vec3 wg = vec3(0,1,0); //geometric normal
    vec3 wm;
    vec3 wo;
    double a2; //roughness squared
    double D_val;

    double wgdotwm = 0;

};

class hittable_pdf : public pdf {
  public:
    hittable_pdf(hittable& _objects,  const point3& _origin) : objects(_objects), origin(_origin)
    {}

    double value(const vec3& direction) const override {
        return objects.pdf_value(origin, direction);
    }

    vec3 generate() override {
        return objects.random(origin);
    }

  private:
    hittable& objects;
    const point3 origin;
};

class mixture_pdf : public pdf {
  public:
    mixture_pdf(shared_ptr<pdf> p0, shared_ptr<pdf> p1) {
        p[0] = p0;
        p[1] = p1;
    }

    double value(const vec3& direction) const override {
        return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
    }

    vec3 generate() override {
        if (random_double() < 0.5)
            return p[0]->generate();
        else
            return p[1]->generate();
    }

  private:
    shared_ptr<pdf> p[2];
    
};




#endif