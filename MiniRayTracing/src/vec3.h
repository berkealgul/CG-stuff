#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

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

	Vec3 operator*=(const double t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
	}

	Vec3 operator/=(const double t)
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

	inline Vec3 unit()
	{
		return *this / length();
	}

	inline double operator[](int i) const { return e[i]; }

	inline double x() const { return e[0]; }
	inline double y() const { return e[1]; }
	inline double z() const { return e[2]; }

private:
	double e[3];

};

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
	return Vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline Vec3 operator-(Vec3& u, Vec3& v)
{
	return Vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

inline Vec3 operator*(Vec3& u, Vec3& v)
{
	return Vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline Vec3 operator*(double t, Vec3& v)
{
	return Vec3(t * v[0], t * v[1], t * v[2]);
}

inline Vec3 operator*(Vec3& v, double t)
{
	return t * v;
}

inline Vec3 operator/(Vec3& v, double t)
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

using Point3 = Vec3;
using Color = Vec3;

#endif // VEC3_H

