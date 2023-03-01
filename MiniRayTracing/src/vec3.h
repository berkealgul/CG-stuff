#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

template <typename T>
class Vec3
{
public:
	Vec3() : e{ 0, 0, 0 } {}
	Vec3(T e1, T e2, T e3) : e{ e1, e2, e3 } {}

	Vec3& operator+=(vec3& v)
	{
		e[0] += v.x();
		e[1] += v.y();
		e[2] += v.z();
		return *this;
	}

	Vec3 operator*=(const T t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
	}

	Vec3 operator/=(const T t)
	{
		return *this *= 1 / t;
	}

	T length () const
	{
		return std::sqrt(length_squared());
	}

	T length_squared() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	inline T operator[](int i) { return e[i]; }

	inline T x() { return e[0]; }
	inline T y() { return e[1]; }
	inline T z() { return e[2]; }

	//inline std::ostream& operator<<(std::ostream& out)
	//{
	//	return out << e[0] << ' ' << e[1] << ' ' << e[2];
	//}

private:
	T e[3];

};


using point3 = Vec3<double>;
using color = Vec3<double>;

#endif // VEC3_H

