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
        bool scatter(const ray& ray_in, const hit_record& rec, color& attentuation, ray& scattered) const override;
};

class metal: public material {
    private:
        color albedo;
        double fuzz;
    public:
        metal(const color& alb, double fz) : albedo(alb), fuzz(fz < 1 ? fz : 1)  {}
        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;
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

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;
};
#endif