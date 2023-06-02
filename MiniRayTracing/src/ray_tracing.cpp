#define STB_IMAGE_IMPLEMENTATION
#include "stb_image_inc.h"
#include "color.h"
#include "vec3.h"
#include "ray.h"
#include "utils.h"
#include "hittableList.h"
#include "material.h"
#include "Sphere.h"
#include "Hittable.h"
#include "Camera.h"
#include "movingSphere.h"
#include "bvhNode.h"
#include "texture.h"
#include "aarect.h"
#include "box.h"
#include "constant_medium.h"

#include <iostream>
#include <chrono>
using namespace std::chrono;

inline int IX(int i, int j, int w)
{
    return j * w + i;
}


HittableList final_scene()
{
    HittableList boxes1;
    auto ground = make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) 
    {
        for (int j = 0; j < boxes_per_side; j++) 
        {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<Box>(Point3(x0, y0, z0), Point3(x1, y1, z1), ground));
        }
    }

    HittableList objects;

    objects.add(make_shared<BVHNode>(boxes1, 0, 1));

    auto light = make_shared<DiffuseLight>(Color(7, 7, 7));
    objects.add(make_shared<XZRect>(123, 423, 147, 412, 554, light));

    auto center1 = Point3(400, 400, 200);
    auto center2 = center1 + Vec3(30, 0, 0);
    auto moving_sphere_material = make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    objects.add(make_shared<MovingSphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(make_shared<Sphere>(Point3(260, 150, 45), 50, make_shared<Dielectric>(1.5)));
    objects.add(make_shared<Sphere>(
        Point3(0, 150, 145), 50, make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)
        ));

    auto boundary = make_shared<Sphere>(Point3(360, 150, 145), 70, make_shared<Dielectric>(1.5));
    objects.add(boundary);
    objects.add(make_shared<constant_medium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
    boundary = make_shared<Sphere>(Point3(0, 0, 0), 5000, make_shared<Dielectric>(1.5));
    objects.add(make_shared<constant_medium>(boundary, .0001, Color(1, 1, 1)));

    auto emat = make_shared<Lambertian>(make_shared<ImageTexture>("earth_map.jpg"));
    objects.add(make_shared<Sphere>(Point3(400, 200, 400), 100, emat));

    HittableList boxes2;
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<Sphere>(Point3::random(0, 165), 10, white));
    }

    objects.add(make_shared<translate>(
        make_shared<rotate_y>(
            make_shared<BVHNode>(boxes2, 0.0, 1.0), 15),
        Vec3(-100, 270, 395)
        )
    );

    return objects;
}


HittableList cornell_box()
{
    HittableList objects;

    auto red = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color(15, 15, 15));

    objects.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<XZRect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));

    shared_ptr<Hittable> box1 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, Vec3(265, 0, 295));
    objects.add(box1);

    shared_ptr<Hittable> box2 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, Vec3(130, 0, 65));
    objects.add(box2);

    return objects;
}


HittableList simple_light() 
{
    HittableList objects;

    auto pertext = Color(.3, .3, .3);
    objects.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    objects.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

    auto difflight = make_shared<DiffuseLight>(Color(4, 4, 4));
    objects.add(make_shared<XYRect>(3, 5, 1, 3, -2, difflight));

    return objects;
}

HittableList earth() 
{
    auto earth_texture = make_shared<ImageTexture>("earth_map.jpg");
    auto earth_surface = make_shared<Lambertian>(earth_texture);
    auto globe = make_shared<Sphere>(Point3(0, 0, 0), 2, earth_surface);

    return HittableList(globe);
}


HittableList random_scene() 
{
    HittableList world;

    auto checker = make_shared<CheckherTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(checker)));


    for (int a = -11; a < 11; a++) 
    {
        for (int b = -11; b < 11; b++) 
        {
            auto choose_mat = random_double();
            Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) 
            {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) 
                {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    auto center2 = center + Vec3(0, random_double(0, .5), 0);
                    world.add(make_shared<MovingSphere>(
                        center, center2, 0.0, 1.0, 0.2, sphere_material));
                    //world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) 
                {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else 
                {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}

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

Color ray_color(const Ray& r, Hittable& world, int depth, const Color& background = Color(0, 0, 0))
{
    HitResult res;

    if (depth <= 0) { return Color(0, 0, 0); }
       
    if (world.hit(r, 0.001, infinity, res)) 
    {
        Ray scattered;
        Color attenuation;
        Color emitted = res.mat_ptr->emitted(res.u, res.v, res.p);

        if (res.mat_ptr->scatter(r, res, attenuation, scattered))
            return emitted + attenuation * ray_color(scattered, world, depth - 1, background);
        else
            return emitted;
    }

    return background;
}


int main()
{
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 1000;
    const int max_depth = 50;


    // World: random_scene(), earth(), simple_light(), cornell_box(), final_scene()
    HittableList world = final_scene();
    Color background(0, 0, 0);

    // Camera
    Point3 lookfrom(478, 278, -600);
    Point3 lookat(278, 278, 0);
    Vec3 vup(0, 1, 0); 
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;

    Camera cam(lookfrom, lookat, vup, 40, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    // image buffer to store pixel colors
    std::vector<Color> image_buffer(image_width * image_height);

    auto start = high_resolution_clock::now();

    int finished_rows = 0;

    // Render
#pragma omp parallel for \
  default(none) \
  shared(image_buffer, finished_rows) \
  num_threads(8)
    for (int j = image_height-1; j >= 0; --j)
    {
        for (int i = 0; i < image_width; i++)
        {
            // Render single pixel
            Color c(0, 0, 0);

            // for anti alising
            for (int s = 0; s < samples_per_pixel; s++)
            {
                double u = double(i + random_double()) / (image_width - 1);
                double v = double(j + random_double()) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                c = c + ray_color(r, world, max_depth, background);
            }

            image_buffer[IX(i, j, image_width)] = c;
        }

        finished_rows++;
        std::cerr << "Row finished " << finished_rows << "/" << image_height << '\n';
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    std::cerr << "Done! in: " << duration.count() << '\n';

    // Write image
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height - 1; j >= 0; --j)
        for (int i = 0; i < image_width; i++)
            write_color(std::cout, image_buffer[IX(i, j, image_width)], samples_per_pixel);

    return 0;
}