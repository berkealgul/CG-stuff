#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "vec3.h"

class Camera
{
public:
	Camera()
	{
		viewport_width = viewport_height * aspect_ratio;
		image_width = static_cast<int>(image_height * aspect_ratio);

		horizontal = Vec3(viewport_width, 0, 0);
		vertical = Vec3(0, viewport_height, 0);
		origin = Vec3(0, 0, 0);
		upper_left_corner = vertical / 2 - horizontal / 2 - Vec3(0, 0, focal_length);
	}

	Ray get_ray(double u, double v) const 
	{
		return Ray(origin, upper_left_corner + horizontal * u - vertical * v);
	}


private:
	double aspect_ratio = 16.0 / 9.0;
	int image_height = 400;
	int image_width;
	double focal_length = 1.0;
	double viewport_height = 2.0;
	double viewport_width;

	Vec3 horizontal;
	Vec3 vertical;
	Vec3 origin;
	Vec3 upper_left_corner;
};


#endif