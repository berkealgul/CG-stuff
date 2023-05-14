#pragma once
#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "utils.h"

#include "hittable.h"
#include "material.h"
#include "texture.h"


class isotropic : public Material 
{
public:
    isotropic(Color c) : albedo(make_shared<SolidColor>(c)) {}
    isotropic(shared_ptr<Texture> a) : albedo(a) {}

    virtual bool scatter(
        const Ray& r_in, const HitResult& rec, Color& attenuation, Ray& scattered
    ) const override 
    {
        scattered = Ray(rec.p, random_in_unit_sphere(), r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

public:
    shared_ptr<Texture> albedo;
};


class constant_medium : public Hittable 
{
public:
    constant_medium(shared_ptr<Hittable> b, double d, shared_ptr<Texture> a)
        : boundary(b),
        neg_inv_density(-1 / d),
        phase_function(make_shared<isotropic>(a))
    {}

    constant_medium(shared_ptr<Hittable> b, double d, Color c)
        : boundary(b),
        neg_inv_density(-1 / d),
        phase_function(make_shared<isotropic>(c))
    {}

    virtual bool hit(
        const Ray& r, double t_min, double t_max, HitResult& rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override 
    {
        return boundary->bounding_box(time0, time1, output_box);
    }

public:
    shared_ptr<Hittable> boundary;
    shared_ptr<Material> phase_function;
    double neg_inv_density;
};

bool constant_medium::hit(const Ray& r, double t_min, double t_max, HitResult& rec) const 
{

    HitResult rec1, rec2;

    if (!boundary->hit(r, -infinity, infinity, rec1))
        return false;

    if (!boundary->hit(r, rec1.t + 0.0001, infinity, rec2))
        return false;

    if (rec1.t < t_min) rec1.t = t_min;
    if (rec2.t > t_max) rec2.t = t_max;

    if (rec1.t >= rec2.t)
        return false;

    if (rec1.t < 0)
        rec1.t = 0;

    const auto ray_length = r.direction().length();
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const auto hit_distance = neg_inv_density * log(random_double());

    if (hit_distance > distance_inside_boundary)
        return false;

    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = r.at(rec.t);
    rec.normal = Vec3(1, 0, 0);  
    rec.front_face = true;   
    rec.mat_ptr = phase_function;

    return true;
}

#endif