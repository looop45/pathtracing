        double check_shadow(point3 intersect, hittable& world)
        {
            vec3 unit = unit_vector(lightDir);
            point3 p = intersect;
            ray shadow_ray(p, unit);
            hit_record temp;

            if (world.hit(shadow_ray, 0.001, infinity, temp))
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }

        color refl_color(vec3 n, vec3 d, point3 p, hittable& world, int depth)
        {
            vec3 x = lightDir;
            vec3 r = d - 2 * n * (dot(d, n));
            hit_record rec;

            ray reflect_ray(p,r);
            if (world.hit(reflect_ray, 0.001, infinity, rec))
            {
                double in_shadow = check_shadow(rec.p, surfaces);
                if (depth > -1)
                {
                    color rcol = refl_color(rec.normal, reflect_ray.direction(), rec.p, surfaces, depth - 1);
                    return rec.material.compute(rec.normal, reflect_ray.direction(), in_shadow, rcol);\
                }
                return rec.material.compute(rec.normal, reflect_ray.direction(), in_shadow);

            }
            return bgColor;
        }

        //returns the color derived from the shading model or the BG
        color ray_color(const ray& r, hittable& world)
        {
            hit_record rec;
            if (world.hit(r, 0, infinity, rec))
            {
                color out;

                return out;
            }
            //no hit, do gradient bg
            return bgColor;
        }
