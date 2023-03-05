#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class Sphere : public Hittable
{
	Sphere() {}
	Sphere(Point3 c, double r) :
		center(c),
		radius(r)
	{}

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
		res.normal = (res.p - center) / radius;

		return true;
	}

private:
	Point3 center;
	double radius;
};

#endif // SPHERE_H