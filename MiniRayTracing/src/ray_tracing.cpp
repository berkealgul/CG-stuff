#include "color.h"
#include "vec3.h"
#include "ray.h"
#include "utils.h"
#include "hittableList.h"
#include "Sphere.h"
#include "Hittable.h"
#include "Camera.h"

#include <iostream>


double hit_sphere(const Point3& center, double radius, const Ray& r) 
{
    Vec3 oc = r.origin() - center;
    double a = r.direction().length_squared();
    double half_b = dot(oc, r.direction());
    double c = oc.length_squared() - radius * radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0) 
    {
        return -1.0;
    }
    else 
    {
        return (-half_b - sqrt(discriminant)) / a;
    }
}

Color ray_color(const Ray& r, Hittable& world)
{
    HitResult res;

    if (world.hit(r, 0, infinity, res)) 
    {
        return 0.5 * (res.normal + Color(1, 1, 1));
    }

    Vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}


int main()
{
    // Image
    double aspect_ratio = 16.0 / 9.0;
    const int image_height = 400;
    const int image_width = static_cast<int>(image_height * aspect_ratio);

    const int sample_per_pixel = 100;

    // World
    HittableList world;
    world.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    // Camera
    Camera cam;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++)
    {
        for (int i = 0; i < image_width; ++i)
        {
            Color c(0, 0, 0);

            for (int s = 0; s < sample_per_pixel; s++)
            {
                double u = double(i + random_double()) / (image_width - 1);
                double v = double(j + random_double()) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                c = c + ray_color(r, world);
            }

            write_color(std::cout, c,  sample_per_pixel);
        }
    }
        
    std::cerr << "Done!" << '\n';
}