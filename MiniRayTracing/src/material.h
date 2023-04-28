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

		scattered = Ray(res.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

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
		scattered = Ray(res.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), res.normal) > 0);
	}

	Color albedo;
	double fuzz;
};


#endif // MATERIAL_H
