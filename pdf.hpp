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
    ggx_pdf(vec3& wo, vec3 w, double roughness)
    {
      uvw.build_from_w(w);
      this->wo = uvw.world_to_local(wo);
      double a = roughness * roughness;
      a2 = a * a;
    }

    //D term
    const double D()
    {
      double wgdotwm2 = wgdotwm * wgdotwm;
      double b = (wgdotwm2 * (a2 - 1.0) + 1.0);
      //double denom_sqrt = ((wgdotwm * wgdotwm) * (a2-1.0) + 1.0);
      //D_val = a2 / (pi * denom_sqrt * denom_sqrt);
      D_val = a2 * inv_pi / (b*b);
      return D_val;
    }

    //pdf of ggx given importance sampling of NDF
    double value(const vec3 &wi) const override 
    {
      return (D_val * wgdotwm)/(4 * dot(wo, wm));
      return 1;
    }

    //sampling direction of NDF for new microfacet normal (half vector)
    vec3 generate() override
    {
      double e0 = random_double();
      double e1 = random_double();

      auto theta = acos(sqrt((1.0 - e0) / ((a2 - 1.0) * e0 + 1.0)));
      auto phi = pi * pi * e1;

      //Spherical to cartesian coordinates
      wm = spherical_to_cartesian(theta, phi);
      wgdotwm = dot(wg, wm);

      //convert to cartesian
      return uvw.local_to_world(wm);
    }

  private:
    const vec3 wg = vec3(0,0,1); //geometric normal
    vec3 wm;
    vec3 wo;
    double a2; //roughness squared
    double D_val;
    onb uvw;
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