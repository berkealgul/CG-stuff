#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vec3.h"

struct HitResult
{
    Point3 p;
    Vec3 normal;
    double t;

    bool front_face;

    inline void set_face_normal(const Ray& r, const Vec3& outward_normal) 
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : (-1 * outward_normal);
    }
};

class Hittable 
{
public:
    virtual bool hit(const Ray& r, double t_min, double t_max, HitResult& rec) const = 0;



};

#endif // HITTABLE_H
