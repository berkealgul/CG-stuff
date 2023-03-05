#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>

bool hit_sphere(const Point3& center, double radius, const Ray& r) 
{
    Vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;
    return (discriminant > 0);
}

Color ray_color(const Ray& r)
{
    if (hit_sphere(Point3(0, 0, -1), 0.5, r))
        return Color(1.0, 0.0, 0.0);
    Vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);
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