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

class dielectric: public material {
    private:
    /* Refractive index in vacumm or air, or the ratio of the material's refractive index 
        over the refractive index of the enclosing media*/
        double refractive_index;
        static double reflectance(double cosine, double refraction_index) {
            // Use schlick's approximation for reflectance.
            auto r0 = (1-refraction_index) / (1+refraction_index);
            r0 = r0*r0;
            return r0 + (1-r0)*std::pow((1-cosine), 5);
        }
    public:
        dielectric(double ri): refractive_index(ri) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
            attenuation = color(1.0, 1.0, 1.0);
            double ri = rec.front_face ? (1.0/refractive_index) : refractive_index;

            vec3 unit_direction = unit_vector(r_in.direction());
            double cost_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = std::sqrt(1.0 - cost_theta*cost_theta);

            bool cannot_refract = ri*sin_theta > 1.0;
            vec3 direction;
            if (cannot_refract || reflectance(cost_theta, ri) > random_double())
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, ri);

            scattered = ray(rec.p, direction);
            return true;
        }
};
#endif