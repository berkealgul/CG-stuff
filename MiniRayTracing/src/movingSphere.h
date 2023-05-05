#pragma once
#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "hittable.h"
#include "utils.h"


class MovingSphere : public Hittable
{
public:
	MovingSphere() {}
	MovingSphere(Point3 cen0, Point3 cen1, double _time0, double _time1, double r, shared_ptr<Material> m)
		: center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m)
	{};

	virtual bool hit(const Ray& r, double t_min, double t_max, HitResult& res) const override
	{
        Vec3 oc = r.origin() - center(r.time());
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if (root < t_min || t_max < root) 
        {
            root = (-half_b + sqrtd) / a;
            if (root < t_min || t_max < root)
                return false;
        }

        res.t = root;
        res.p = r.at(res.t);
        auto outward_normal = (res.p - center(r.time())) / radius;
        res.set_face_normal(r, outward_normal);
        res.mat_ptr = mat_ptr;

        return true;
	}

	Point3 center(double time) const 
	{
		return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
	}

public:
	Point3 center0, center1;
	double time0, time1;
	double radius;
	shared_ptr<Material> mat_ptr;
};

#endif