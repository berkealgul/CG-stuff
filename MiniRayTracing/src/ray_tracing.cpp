#include "color.h"
#include "vec3.h"
#include "ray.h"

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

Color ray_color(const Ray& r)
{
    double t = hit_sphere(Point3(0, 0, -1), 0.5, r);
    if (t > 0)
    {
        Vec3 N = unit_vector(r.at(t) - Vec3(0, 0, -1));
        return 0.5 * (N + 1);
    }
    Vec3 unit_direction = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}


int main()
{
    // Image
    double aspect_ratio = 16.0 / 9.0;
    const int image_height = 400;
    const int image_width = static_cast<int>(image_height * aspect_ratio);

    // Camera
    double focal_length = 1.0;

    double viewport_height = 2.0;
    double viewport_width = viewport_height * aspect_ratio;

    Vec3 horizontal(viewport_width, 0, 0);
    Vec3 vertical(0, viewport_height, 0);
    Vec3 origin(0, 0, 0);

    Vec3 upper_left_corner = vertical / 2 - horizontal / 2 - Vec3(0, 0, focal_length);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++)
    {
        for (int i = 0; i < image_width; ++i)
        {
            double u = double(i) / (image_width - 1);
            double v = double(j) / (image_height - 1);
            Vec3 direction = upper_left_corner + horizontal * u - vertical * v;
            Ray r(origin, direction);
            Color c = ray_color(r);
            write_color(std::cout, c);
        }
    }
        
    std::cerr << "Done!" << '\n';
}