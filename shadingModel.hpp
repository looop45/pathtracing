#ifndef SMODEL_H
#define SMODEL_H

#include "vec3.hpp"
#include "vec2.hpp"
#include "image_texture.hpp"
#include <math.h>

class shadingModel
{
    public:
        shadingModel() {}
        /*
        Kd -> diffuse gain
        Ks -> specular gain
        Ka -> ambient gain
        Od -> Diffuse color
        Oa -> Ambient color
        Os -> Specular color
        kgls -> Specular exponent
        refl -> Reflection gain
        roughness -> Reflection glossiness
        Kt -> transmission gain
        IOR -> transmission index of refraction 
        refr_rough -> refraction roughness
        */
        shadingModel(double Kd, double Ks, double Ka, color Od, color Oa, color Os, double kgls, double refl, double roughness, double Kt, double IOR, double refr_rough)
        {
            //diffuse
            this->Kd = Kd;
            this->Od = Od;

            //ambient
            this->Ka = Ka;
            this->Oa = Oa;

            //specular
            this->Ks = Ks;
            this->kgls = kgls;
            this->Os = Os;

            //reflect
            this->refl = refl;
            this->roughness = roughness;

            //transmission
            this->IOR = IOR;
            this->Kt = Kt;
            this->refr_rough = refr_rough;
            
        }

        shadingModel(double Kd, double Ks, double Ka, image_texture* Od_image, color Oa, color Os, double kgls, double refl, double roughness, double Kt, double IOR, double refr_rough)
        {
            //diffuse
            this->Kd = Kd;
            this->Od_image = Od_image;
            this->Od_hasImage = true;

            //ambient
            this->Ka = Ka;
            this->Oa = Oa;

            //specular
            this->Ks = Ks;
            this->kgls = kgls;
            this->Os = Os;

            //reflect
            this->refl = refl;
            this->roughness = roughness;

            //transmission
            this->IOR = IOR;
            this->Kt = Kt;
            this->refr_rough = refr_rough;
        }

        bool is_reflective()
        {
            return this->refl;
        }

        bool is_transmissive()
        {
            return this->Kt;
        }

        color compute_diffuse(vec3 normal, vec3 V, vec3 L, color Ip, vec2 uv)
        {
            color Id;
            //diffuse
            if (this->Od_hasImage)
            {
                Id = Kd * Ip * Od_image->value(uv[0], uv[1]) * max((double)0, dot(normal, L));
            }
            else 
            { 
                Id = Kd * Ip * Od * max((double)0, dot(normal, L));
            }
            return clamp(Id);
        }

        color compute_spec(vec3 normal, vec3 V, vec3 L, color Ip)
        {
            //specular
            vec3 R = unit_vector(2 * (dot(normal, -L)) * normal - (-L));
            color Is = Ks * Ip * Os * pow(max((double)0, dot(V, R)), kgls);

            return clamp(Is);
        }

        color compute_ambient(vec2 uv)
        {
            color Ia;
            //ambient
            if (this->Od_hasImage)
            {
                Ia = Ka * Oa * Od_image->value(uv[0], uv[1]);
            }
            else 
            { 
                Ia = Ka * Oa * Od;
            }
            return clamp(Ia);
        }

        color compute(vec3 normal, vec3 V, vec3 L, color Ip)
        {
            //diffuse
            color Id = Kd * Ip * Od * max((double)0, dot(normal, L));

            //specular
            vec3 R = unit_vector(2 * (dot(normal, L)) * normal - L);
            color Is = Ks * Ip * Os * pow(max((double)0, dot(V, R)), kgls);

            //reflective
                        //ambient
            color Ia = Ka * Oa * Od;
            

            return clamp(Is + Ia + Id);
        }

        //reflect
        double refl;
        double roughness;

        double IOR;        //transmission
        double Kt;
        double refr_rough;


    protected:

        //diffuse
        double Kd;
        color Od;
        image_texture* Od_image = nullptr;
        bool Od_hasImage = false;

        //ambient
        double Ka;
        color Oa;

        //specular
        double Ks;
        double kgls;
        color Os;


};

#endif
