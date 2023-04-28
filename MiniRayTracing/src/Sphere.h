#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include "material.h"

class Sphere : public Hittable
{
public:
	Sphere() {}
	Sphere(Point3 c, double r, shared_ptr<Material> m) :
		center(c),
		radius(r),
		mat_ptr(m)
	{};

	virtual bool hit(const Ray& r, double t_min, double t_max, HitResult& res) const override
	{
		Vec3 oc = r.origin() - center;
		double a = r.direction().length_squared(); // same as dot(v.direction(), v.direction())
		double hb = dot(r.direction(), oc);
		double c = dot(oc, oc) - radius * radius;

		double discriminant = hb * hb - a * c;

		if (discriminant < 0) { return false; }

		double sd = sqrt(discriminant);

		// Find the nearest root that lies in the acceptable range.
		double root = (-hb - sd) / a;
		if (root < t_min || t_max < root) 
		{
			// if first root fails try second root
			root = (-hb + sd) / a; 
			if (root < t_min || t_max < root)
			{
				return false;
			}
		}

		res.t = root;
		res.p = r.at(root);
		Vec3 outward_normal = (res.p - center) / radius;
		res.set_face_normal(r, outward_normal);
		res.mat_ptr = mat_ptr;

		return true;
	}

private:
	Point3 center;
	double radius;
	shared_ptr<Material> mat_ptr;
};

#endif // SPHERE_H