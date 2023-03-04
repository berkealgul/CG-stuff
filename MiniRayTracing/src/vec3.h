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
		e[0] += v.x();
		e[1] += v.y();
		e[2] += v.z();
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

	double length () const
	{
		return std::sqrt(length_squared());
	}

	double length_squared() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	inline double operator[](int i) { return e[i]; }

	inline double x() { return e[0]; }
	inline double y() { return e[1]; }
	inline double z() { return e[2]; }

	//inline std::ostream& operator<<(std::ostream& out)
	//{
	//	return out << e[0] << ' ' << e[1] << ' ' << e[2];
	//}

private:
	double e[3];

};


using Point3 = Vec3;
using Color = Vec3;

#endif // VEC3_H

