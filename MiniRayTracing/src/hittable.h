#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vec3.h"

struct HitResult
{
    Point3 p;
    Vec3 normal;
    double t;
};

class Hittable 
{
public:
    virtual bool hit(const Ray& r, double t_min, double t_max, HitResult& rec) const = 0;

};

#endif // HITTABLE_H
