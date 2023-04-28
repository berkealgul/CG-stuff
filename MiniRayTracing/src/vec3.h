#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>
#include "utils.h"

class Vec3
{
public:
	Vec3() : e{ 0, 0, 0 } {}
	Vec3(double e1, double e2, double e3) : e{ e1, e2, e3 } {}

	Vec3& operator+=(Vec3& v)
	{
		e[0] += v[0];
		e[1] += v[1];
		e[2] += v[2];
		return *this;
	}

	Vec3& operator*=(const double t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
	}

	Vec3& operator/=(const double t)
	{
		return *this *= 1 / t;
	}

	double length() const
	{
		return std::sqrt(length_squared());
	}

	double length_squared() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	//inline Vec3 unit()
	//{
	//	return *this / length();
	//}

	inline double operator[](int i) const { return e[i]; }

	inline double x() const { return e[0]; }
	inline double y() const { return e[1]; }
	inline double z() const { return e[2]; }

	inline static Vec3 random()
	{
		return Vec3(random_double(), random_double(), random_double());
	}

	inline static Vec3 random(double min, double max)
	{
		return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

	bool near_zero() const 
	{
		// Return true if the vector is close to zero in all dimensions.
		const auto s = 1e-8;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}

private:
	double e[3];

};

using Point3 = Vec3;
using Color = Vec3;

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
	return Vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline Vec3 operator+(const Vec3& u, double t)
{
	return Vec3(u[0] + t, u[1] + t, u[2] + t);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) 
{
	return Vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
	return Vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline Vec3 operator*(double t, const Vec3& v)
{
	return Vec3(t * v[0], t * v[1], t * v[2]);
}

inline Vec3 operator*(const Vec3& v, double t)
{
	return t * v;
}

inline Vec3 operator/(Vec3 v, double t) 
{
	return (1 / t) * v;
}

inline double dot(const Vec3& u, const Vec3& v) 
{
	return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline Vec3 cross(const Vec3& u, const Vec3& v) 
{
	return Vec3(
		u[1] * v[2] - u[2] * v[1],
		u[2] * v[0] - u[0] * v[2],
		u[0] * v[1] - u[1] * v[0]);
}

inline Vec3 unit_vector(Vec3 v) 
{
	return v / v.length();
}

Vec3 random_in_unit_sphere() 
{
	while (true)
	{
		auto p = Vec3::random(-1, 1);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

Vec3 random_in_hemisphere(const Vec3& normal) 
{
	Vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
		return in_unit_sphere;
	else
		return -1 * in_unit_sphere;
}

Vec3 random_unit_vector()
{
	return unit_vector(Vec3::random(-1, 1));
}

Vec3 reflect(const Vec3& v, const Vec3& n) {
	return v - 2 * dot(v, n) * n;
}

#endif // VEC3_H

