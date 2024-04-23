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
        shared_ptr<hittable> pdf_ptr;
        color brdf;
        double pdf_val;
        onb uvw;
};

class material
{
    public:
        material() {}

        virtual color emitted(const ray& r_in, const hit_record& rec, double u, double v, const point3& p) const 
        {
            return color(0,0,0);
        }

        virtual bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) 
        {
            return false;
        }

};

class lambertian : public material
{
    public:
        lambertian(const shared_ptr<texture> a) : albedo(a) {}

        color brdf(const ray& r_in, const hit_record& rec, const ray& scattered) const 
        {
            auto cos_theta = dot(rec.normal, unit_vector(scattered.direction()));
            return color(inv_pi * cos_theta);
        }

        bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) 
        {
            scatter_record* scat_ptr = &srec;
            srec.attenuation = albedo->value(rec.uv[0], rec.uv[1]);
            //cosine_pdf test(rec.normal);
            // cosine_pdf test2(rec.normal);
            //pdf* ptr = new cosine_pdf(rec.normal);
            //cosine_pdf thispdf(rec.normal);
            //srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);

            //srec.scattered_ray = ray(rec.p, srec.pdf_ptr->generate());
            srec.scattered_ray = ray(rec.p, vec3(0,1,0));

            srec.brdf = brdf(r_in, rec, srec.scattered_ray);
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

/*
//Cook-Torrance specular model
class specular_CT : public material
{
    specular_CT(shared_ptr<texture> rough, shared_ptr<texture> basecol, shared_ptr<texture> metal)
    {
        roughness = rough;
        basecolor = basecol;
        metalness = metal;
    }

    specular_CT(double rough) : roughness(make_shared<solid_color>(color(rough, rough, rough))) {}

    color brdf(const hit_record& rec, vec3& wm, vec3& wo, vec3& wi, double a2, double D) const 
    {
        double widotwm = dot(wi, wm);
        //if not in the hemisphere, contribution is zero
        if (dot(wg, wi) < 0 || widotwm < 1)
        {
            return color(0,0,0);
        }

        double dotNL = dot(wg, wi);
        double dotNV = dot(wg, wo);

        //schlick fresnel term
        color f0_dielectric(0.04, 0.04, 0.04);
        color f0 = lerp(f0_dielectric, basecolor->value(rec.uv[0], rec.uv[1]), metalness->value(rec.uv[0], rec.uv[1])[0]);
        color F = f0 + (color(1,1,1) - f0)*(1-pow(dotNV, 5));

        //Smith shadow masking
        double denomA = dotNV * sqrt(a2 + (1 - a2) * dotNL * dotNL);
        double denomB = dotNL * sqrt(a2 + (1 - a2) * dotNV * dotNV);
        double G = 2 * dotNL * dotNV / (denomA + denomB);

        return (F * G * D) / (4 * dot(wi, wg) * dot(wo, wg));
    }

    bool scatter(const ray& r_in, hit_record& rec, scatter_record srec) 
    {
        srec.attenuation = color(1,1,1);
        srec.uvw.build_from_w(rec.normal);

        vec3 wo = srec.uvw.world_to_local(r_in.direction());

        double a = roughness->value(rec.uv[0], rec.uv[1])[0];

        srec.pdf_ptr = make_shared<ggx_pdf>(wo, a);
        vec3 wm = srec.pdf_ptr->generate();

        //reflect
        vec3 wi = 2 * dot(wo, wm) * wm - wo;
        srec.wi = srec.uvw.local_to_world(srec.wi);

        srec.scattered_ray = ray(rec.p, srec.wi);

        //D term
        //ggx_pdf *ggx = static_cast<ggx_pdf*>(srec.pdf_ptr);
        //double D = ggx->D();
        double D = 1.0;

        //pdf_val
        srec.pdf_val = srec.pdf_ptr->value(srec.wi);

        //brdf
        srec.brdf = this->brdf(rec, wm, wo, wi, a*a, D);

        //pdf value
        return true;
    }

    private:
        shared_ptr<texture> roughness;
        shared_ptr<texture> basecolor;
        shared_ptr<texture> metalness;
        const vec3 wg = vec3(0,1,0);
};
*/
#endif
