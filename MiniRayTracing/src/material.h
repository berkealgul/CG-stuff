#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include "utils.h"

struct HitResult;

class Material
{
public:
	virtual bool scatter(
		const Ray& ray_in, const HitResult& res, Color& attenuation, Ray& scattered
	) const = 0;
};


class Lambertian : public Material
{
public:
	Lambertian(const Color& a) : albedo(a) {}

	virtual bool scatter(
		const Ray& ray_in, const HitResult& res, Color& attenuation, Ray& scattered
	) const override
	{
		auto scatter_direction = res.normal + random_unit_vector();

		if (scatter_direction.near_zero())
			scatter_direction = res.normal;

		scattered = Ray(res.p, scatter_direction, ray_in.time());
		attenuation = albedo;
		return true;
	}

public:
	Color albedo; 
};


class Metal : public Material
{
public:
	Metal(const Color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(
		const Ray& ray_in, const HitResult& res, Color& attenuation, Ray& scattered
	) const override
	{
		Vec3 reflected = reflect(unit_vector(ray_in.direction()), res.normal);
		scattered = Ray(res.p, reflected + fuzz * random_in_unit_sphere(), ray_in.time());
		attenuation = albedo;
		return (dot(scattered.direction(), res.normal) > 0);
	}

public:
	Color albedo;
	double fuzz;
};


class Dielectric : public Material
{
public:
	Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

	virtual bool scatter(
		const Ray& ray_in, const HitResult& res, Color& attenuation, Ray& scattered
	) const override
	{
		attenuation = Color(1.0, 1.0, 1.0);
		double refraction_ratio = res.front_face ? (1.0 / ir) : ir;

		Vec3 unit_direction = unit_vector(ray_in.direction());

		double cos_theta = fmin(dot(-unit_direction, res.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = refraction_ratio * sin_theta > 1.0;
		Vec3 direction;

		if (cannot_refract) 
			direction = reflect(unit_direction, res.normal);
		else
			direction = refract(unit_direction, res.normal, refraction_ratio);

		scattered = Ray(res.p, direction, ray_in.time());
		return true;
	}

public:
	double ir;

private:
	static double reflectance(double cosine, double ref_idx) 
	{
		// Use Schlick's approximation for reflectance.
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};


#endif // MATERIAL_H
