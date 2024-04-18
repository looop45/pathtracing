#ifndef MATERIAL_H
#define MATERIAL_H

#include "utilities.hpp"
#include "texture.hpp"
#include "pdf.hpp"
#include "hittable.hpp"
#include <math.h>

class hit_record;

//struct to hold individual scattering values for individual hits
struct scatter_record
{
    public:
        color attenuation;
        ray scattered_ray;
        vec3 wm;
        vec3 wi;
        shared_ptr<pdf> pdf_ptr;
        double brdf;
        double pdf_val;
};

class material
{
    public:
        material() {}

        virtual color emitted(const ray& r_in, const hit_record& rec, double u, double v, const point3& p) const 
        {
            return color(0,0,0);
        }

        virtual bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const 
        {
            return false;
        }

};

class lambertian : public material
{
    public:
        lambertian(const shared_ptr<texture> a) : albedo(a) {}

        double brdf(const ray& r_in, const hit_record& rec, const ray& scattered) const 
        {
            auto cos_theta = dot(rec.normal, unit_vector(scattered.direction()));
            return inv_pi * cos_theta;
        }

        bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const 
        {
            srec.attenuation = albedo->value(rec.uv[0], rec.uv[1]);
            srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal, srec);
            return true;
        }        

    private:
        shared_ptr<texture> albedo;
};






class emissive : public material
{
    public:
        emissive(shared_ptr<texture> a, double intensity) : emit(a) {this->intensity = intensity;}
        emissive(color c, double intensity) : emit(make_shared<solid_color>(c)) {this->intensity = intensity;}

        color emitted(const ray& r_in, const hit_record& rec, double u, double v, const point3& p) const override 
        {
            return emit->value(u, v) * intensity;
        }

    private:
        shared_ptr<texture> emit;
        double intensity = 1;
};

//Cook-Torrance specular model
class specular_CT : public material
{
    specular_CT(shared_ptr<texture> rough) : roughness(rough) {}
    specular_CT(double rough) : roughness(make_shared<solid_color>(color(rough, rough, rough))) {}

    double brdf(const hit_record& rec, vec3& wm, vec3& wo, vec3& wi, double a2, double D) const 
    {
        double widotwm = dot(wi, wm);
        //if not in the hemisphere, contribution is zero
        if (dot(wg, wi) < 0 || widotwm < 1)
        {
            return 0;
        }

        //schlick fresnel term. White reflections for now
        vec3 f0 = 

        //Smith shadow masking
        double dotNL = dot(wg, wi);
        double dotNV = dot(wg, wo);

        double denomA = dotNV * sqrt(a2 + (1 - a2) * dotNL * dotNL);
        double denomB = dotNL * sqrt(a2 + (1 - a2) * dotNV * dotNV);
        double G = 2 * dotNL * dotNV / (denomA + denomB);

        return (F * G * D) / (4 * dot(wi, wg) * dot(wo, wg))
    }

    bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const 
    {
        srec.attenuation = color(1,1,1);
        uvw.build_from_w(rec.normal);

        const vec3 wo = uvw.world_to_local(r_in.direction());

        double a = roughness->value(rec.uv[0], rec.uv[1])[0];

        srec.pdf_ptr = make_shared<ggx_pdf>(wo, a, srec);
        const vec3 wm = srec.pdf_ptr->generate();

        //reflect
        const vec3 wi = 2 * dot(wo, wm) * wm - wo;
        srec.wi uvw.local_to_world(srec.wi);

        //D term
        double D = srec.pdf_ptr->D();

        //pdf_val
        srec.pdf_val = srec.pdf_ptr->value();

        //brdf
        srec.brdf = this->brdf(rec, wm, wo, wi, a*a, D);

        //pdf value
        return true;
    }

    private:
        onb uvw;
        shared_ptr<texture> roughness;
        const vec3 wg = vec3(0,1,0);
}

class glossy : public material
{


}

#endif
