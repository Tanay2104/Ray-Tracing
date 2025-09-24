#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
class material {
    public: 
        virtual ~material() = default;
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const {
            return false;
        }
};

class lambertian : public material {
    private:
        color albedo;
        
    public:
        lambertian(const color& alb) : albedo(alb){}
        bool scatter(const ray& ray_in, const hit_record& rec, color& attentuation, ray& scattered) const override {
            auto scatter_direction = rec.normal + random_unit_vector();

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;
            scattered = ray(rec.p, scatter_direction);
            attentuation = albedo;
            return true;
        }
};

class metal: public material {
    private:
        color albedo;
        double fuzz;
    public:
        metal(const color& alb, double fz) : albedo(alb), fuzz(fz < 1 ? fz : 1)  {}
        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

};
#endif