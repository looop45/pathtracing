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

        color brdf(const ray& r_in, const hit_record& rec, const ray& scattered, const color albedo) const 
        {
            auto cos_theta = dot(rec.normal, unit_vector(scattered.direction()));
            return albedo * color(inv_pi * cos_theta);
        }

        bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) override
        {
            //scatter_record* scat_ptr = &srec;
            srec.attenuation = albedo->value(rec.uv[0], rec.uv[1]);
            //cosine_pdf test(rec.normal);
            // cosine_pdf test2(rec.normal);
            //pdf* ptr = new cosine_pdf(rec.normal);
            //cosine_pdf thispdf(rec.normal);
            srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);

            srec.scattered_ray = ray(rec.p, srec.pdf_ptr->generate());
            //srec.scattered_ray = ray(rec.p, vec3(0,1,0));
            srec.pdf_val = srec.pdf_ptr->value(srec.scattered_ray.direction());
            srec.brdf = brdf(r_in, rec, srec.scattered_ray, srec.attenuation);
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
    public:
        specular_CT(shared_ptr<texture> rough, shared_ptr<texture> basecol, shared_ptr<texture> metal)
        {
            roughness = rough;
            basecolor = basecol;
            metalness = metal;
        }

        specular_CT(double rough) : roughness(make_shared<solid_color>(color(rough, rough, rough))) {}

        color F_Schlick(float cosTheta, vec3 F0)
        {
            double exp = pow(1.0 - cosTheta, 5.0);
            return F0 + (1.0 - F0) * pow(1.0 - abs(cosTheta), 5.0);
        }

        double D_GGX(double NoH, double a)
        {
            double a2 = a * a;
            double NoH2 = NoH * NoH;
            double b = (NoH2 * (a2 - 1.0) + 1);
            return a2 * inv_pi / clamp(b*b, 0.001, infinity);
        }

        double G_Smith(double NoL, double NoV, double roughness)
        {
            return G1(NoL, roughness) * G1(NoV, roughness);
        }

        double G1(double NoV, double roughness)
        {
            double alpha = roughness * roughness;
            float k = alpha / 2.0;
            return max(NoV, 0.001) / (NoV * (1.0 - k) + k);
        }

        color brdf(const hit_record& rec, scatter_record& srec, vec3& wm, vec3& wo, vec3& wi, double roughness, double metalness, color basecolor) 
        {
            double NoL = clamp(dot(rec.normal, wi), 0.0, 1.0);
            double NoV = dot(rec.normal, -wo);
            double NoH = clamp(dot(rec.normal, wm), 0.0, 1.0);
            double VoH = dot(-wo, wm);

            double D = D_GGX(NoH, roughness * roughness);

            srec.pdf_val = (D * dot(wm, rec.normal)) / (4 * abs(dot(-wo, wm)));
            //srec.pdf_val = 1.0;
            double G = G_Smith(NoL, NoV, roughness);

            color F0_dielectric(0.16);
            color F0 = lerp(F0_dielectric, basecolor, metalness);

            color F = F_Schlick(VoH, F0);
            //srec.pdf_val = 1.0;
            //srec.attenuation = D/srec.pdf_val;
            //return color(1);
            color numer = (F * D * G);
            double denom = (4.0 * max(NoV, 0.001));
            color R = numer / denom;
            return R;
        }

        bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) override
        {
            srec.attenuation = color(1);

            vec3 wo = r_in.direction();

            double rough = roughness->value(rec.uv[0], rec.uv[1])[0];
            double metal = metalness->value(rec.uv[0], rec.uv[1])[0];
            color col = basecolor->value(rec.uv[0], rec.uv[1])[0];

            srec.pdf_ptr = make_shared<ggx_pdf>(wo, rec.normal, rough);

            //get microfacet normal (half vector)
            vec3 wm = srec.pdf_ptr->generate();

            //reflect across microfacet normal
            vec3 wi = wo - 2 * wm * dot(wo, wm);

            srec.scattered_ray = ray(rec.p, wi);

            srec.brdf = this->brdf(rec, srec, wm, wo, wi, rough, metal, col);
            //srec.brdf = 1;
            //srec.brdf = srec.brdf / srec.pdf_val;
            //srec.pdf_val = 1;

            return true;
        }

    private:
        shared_ptr<texture> roughness;
        shared_ptr<texture> basecolor;
        shared_ptr<texture> metalness;
        const vec3 wg = vec3(0,0,1);
};

class glossy : public material
{
    public:
        glossy (shared_ptr<texture> rough, shared_ptr<texture> basecol, shared_ptr<texture> metal)
        {
            roughness = rough;
            basecolor = basecol;
            metalness = metal;
        }

        color F_Schlick(float cosTheta, vec3 F0)
        {
            double exp = pow(1.0 - cosTheta, 5.0);
            return F0 + (1.0 - F0) * pow(1.0 - abs(cosTheta), 5.0);
        }

        bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) override
        {
            double rough = roughness->value(rec.uv[0], rec.uv[1])[0];
            double metal = metalness->value(rec.uv[0], rec.uv[1])[0];
            color col = basecolor->value(rec.uv[0], rec.uv[1])[0];

            //probability based on fresnel
            double p = random_double();
            color F0_dielectric(0.16);
            color F0 = lerp(F0_dielectric, col, metal);
            double threshold = F_Schlick(dot(-r_in.direction(), rec.normal), F0)[0];
            
            if (p > threshold)
            {
                lambertian lamb(basecolor);
                return lamb.scatter(r_in, rec, srec);
            }
            else
            {
                specular_CT spec(roughness, basecolor, metalness);
                return spec.scatter(r_in, rec, srec);
            }
        }

    private:
        shared_ptr<texture> roughness;
        shared_ptr<texture> basecolor;
        shared_ptr<texture> metalness;
};

class glass : public material
{
    public:
        glass (shared_ptr<texture> rough, double IOR)
        {
            roughness = rough;
            this->IOR = IOR;
        }

        color F_Schlick(float cosTheta, vec3 F0)
        {
            double exp = pow(1.0 - cosTheta, 5.0);
            return F0 + (1.0 - F0) * pow(1.0 - abs(cosTheta), 5.0);
        }

        bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) override
        {
            double rough = roughness->value(rec.uv[0], rec.uv[1])[0];

            //probability based on fresnel
            double p = random_double();
            color F0(0.04);
            double threshold = F_Schlick(dot(-r_in.direction(), rec.normal), F0)[0];
            
            if (p > threshold)
            {
                double n2 = this->IOR;
                double n1 = 1;

                vec3 wo = -r_in.direction();

                if (dot(wo, rec.normal) < 0)
                {
                    n1 = this->IOR;
                    n2 = 1;
                }

                double nR = n1/n2;
                ggx_pdf ggx(wo, rec.normal, roughness->value(rec.uv[0], rec.uv[1])[0]);
                vec3 N = ggx.generate();

                vec3 T_dir = (nR * (dot(N,wo)) - sqrt(1-pow(nR, 2) * (1-pow(dot(N, wo), 2)))) * N - nR * wo;
                srec.scattered_ray = ray(rec.p, T_dir);
                srec.brdf = 1;
                srec.pdf_val = 0.5;
                return true;
            }
            else
            {
                shared_ptr<solid_color> blackcol = make_shared<solid_color>(0.001,0.001,0.001);

                specular_CT spec(roughness, blackcol, blackcol);
                return spec.scatter(r_in, rec, srec);
            }
        }

    private:
        shared_ptr<texture> roughness;
        double IOR;
};

#endif
