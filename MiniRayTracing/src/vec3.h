#ifndef VEC3_H
#define VEC3_H

#include <cmath>

template <typename T>
class vec3
{
public:
	vec3() : e{ 0, 0, 0 } {}
	vec3(T e1, T e2, T e3) : e{ e1, e2, e3 } {}

	vec3& operator+=(vec3& v)
	{
		e[0] += v.x();
		e[1] += v.y();
		e[2] += v.z();
		return *this;
	}

	vec3 operator*=(const T t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
	}

	vec3 operator/=(const T t)
	{
		return *this *= 1 / t;
	}

	T length () const
	{
		return sqrt(length_squared());
	}

	T length_squared() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	inline T operator[](int i) { return e[i]; }

	inline T x() { return e[0]; }
	inline T y() { return e[1]; }
	inline T z() { return e[2]; }

private:
	T e[3];

};


using point3 = vec3<double>;
using color = vec3<double>;

#endif // VEC3_H

