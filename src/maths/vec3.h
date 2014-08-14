#ifndef VEC_MATH_VEC3_H
#define VEC_MATH_VEC3_H

#include <cmath>
#include <sstream>
#include <cstdio>

#include <functional>

#include "vec2.h"

class quat;

template<typename T>
class vec3Templated {
	public:
		union {
			struct {
				T x, y, z;
			};

			T data[3];
		};

		vec3Templated() {
			x = y = z = 0;
		}

		vec3Templated(T f) {
			x = y = z = f;
		}

		vec3Templated(T xx, T yy, T zz) {
			x = xx;
			y = yy;
			z = zz;
		}

		vec3Templated<T>(const vec2Templated<T>& v, T zz) {
			x = v.x;
			y = v.y;
			z = zz;
		}

		template<class S>
		vec3Templated(const vec3Templated<S>& v) {
			x = v.x;
			y = v.y;
			z = v.z;
		}

		const vec3Templated<T>& operator =(const vec3Templated<T>& v2) {
			x = v2.x;
			y = v2.y;
			z = v2.z;

			return *this;
		}

		//do
		void set(T xx, T yy, T zz) {
			x = xx;
			y = yy;
			z = zz;
		}

		void normalize() {
			*this /= length();
		}

		/*void rotate(const vec3Templated<T>& center, T theta, axis_enum a) {
			T sinAng = sin(theta);
			T cosAng = cos(theta);

			vec3Templated<T> v = *this - center;

			switch (a) {
			case axisX:
				y = center.y + v.y * cosAng - v.z * sinAng;
				z = center.z + v.y * sinAng + v.z * cosAng;
				break;

			case axisY:
				x = center.x + v.z * sinAng + v.x * cosAng;
				z = center.z + v.z * cosAng - v.x * sinAng;
				break;

			case axisZ:
				x = center.x + v.x * cosAng - v.y * sinAng;
				y = center.y + v.x * sinAng + v.y * cosAng;
				break;
			}
		}*/

		void operator +=(const vec3Templated<T>& v2) {
			x += v2.x;
			y += v2.y;
			z += v2.z;
		}
		void operator -=(const vec3Templated<T>& v2) {
			x -= v2.x;
			y -= v2.y;
			z -= v2.z;
		}
		void operator *=(T f) {
			x *= f;
			y *= f;
			z *= f;
		}
		void operator /=(T f) {
			x /= f;
			y /= f;
			z /= f;
		}

		//get
		template<class S>
		operator vec3Templated<S>(){
			return vec3Templated<S>((S)x, (S)y, (S)z);
		}

		T length() {
			return sqrt(sqrLength());
		}

		T sqrLength() const {
			return x * x + y * y + z * z;
		}
		T distanceTo(const vec3Templated<T>& rhs) const {
			vec3Templated<T> diff = *this - rhs;
			return sqrt(diff * diff);
		}
		T angleWith(vec3Templated<T>& v2) {
			return acos((*this * v2) / (length() * v2.length()));
		}

		vec3Templated<T> operator -() const {
			return vec3Templated<T>(-x, -y, -z);
		}

		vec3Templated<T> normal() const {
			vec3Templated<T> a(*this);
			a.normalize();
			return a;
		}
		vec3Templated<T> abs() {
			return vec3Templated<T>(fabs(x), fabs(y), fabs(z));
		}

		vec3Templated<T> operator +(const vec3Templated<T>& v2) const {
			return vec3Templated<T>(x + v2.x, y + v2.y, z + v2.z);
		}
		vec3Templated<T> operator -(const vec3Templated<T>& v2) const {
			return vec3Templated<T>(x - v2.x, y - v2.y, z - v2.z);
		}
		vec3Templated<T> operator /(const vec3Templated<T>& v2) const {
			return vec3Templated<T>(x / v2.x, y / v2.y, z / v2.z);
		}
		vec3Templated<T> operator *(T f) const {
			return vec3Templated<T>(x * f, y * f, z * f);
		}
		vec3Templated<T> operator /(T f) const {
			return vec3Templated<T>(x / f, y / f, z / f);
		}

		T operator *(const vec3Templated<T>& v2) const {
			return (x * v2.x + y * v2.y + z * v2.z);
		}

		vec3Templated<T> operator ^(const vec3Templated<T>& v2) const {
			return vec3Templated<T>(y * v2.z - z * v2.y, //
			/*        */z * v2.x - x * v2.z, //
			/*        */x * v2.y - y * v2.x);
		}

		quat operator *(const quat& q) const;

		//all
		bool operator ==(const vec3Templated<T>& v2) const {
			return (x == v2.x and y == v2.y and z == v2.z);
		}
		bool operator <(const vec3Templated<T>& v2) const {
			return (x < v2.x and y < v2.y and z < v2.z);
		}
		bool operator <=(const vec3Templated<T>& v2) const {
			return (x <= v2.x and y <= v2.y and z <= v2.z);
		}
		bool operator >(const vec3Templated<T>& v2) const {
			return (x > v2.x and y > v2.y and z > v2.z);
		}
		bool operator >=(const vec3Templated<T>& v2) const {
			return (x >= v2.x and y >= v2.y and z >= v2.z);
		}

		T minComponent() const {
			return fmin(fmin(x, y), z);
		}
		T maxComponent() const {
			return fmax(fmax(x, y), z);
		}
};

template<typename T>
inline vec3Templated<T> operator*(T f, const vec3Templated<T>& v) {
	return v * f;
}

template<typename T>
inline vec3Templated<T> minVec(const vec3Templated<T>& v1, const vec3Templated<T>& v2) {
	vec3Templated<T> v(v1);
	if (v2.x < v1.x) v.x = v2.x;
	if (v2.y < v1.y) v.y = v2.y;
	if (v2.z < v1.z) v.z = v2.z;

	return v;
}

template<typename T>
inline vec3Templated<T> maxVec(const vec3Templated<T>& v1, const vec3Templated<T>& v2) {
	vec3Templated<T> v(v1);
	if (v2.x > v1.x) v.x = v2.x;
	if (v2.y > v1.y) v.y = v2.y;
	if (v2.z > v1.z) v.z = v2.z;

	return v;
}

template<typename T>
inline vec3Templated<T> computeNormal(const vec3Templated<T>& a, const vec3Templated<T>& b, const vec3Templated<T>& c) {
	vec3Templated<T> A = a - b;
	vec3Templated<T> B = a - c;

	return (A ^ B).normal();
}


typedef vec3Templated<float> vec3;
typedef vec3Templated<float> vec3f;
typedef vec3Templated<int> vec3i;

namespace std {
	//specify a hashing function for vec3
	//this gives good results for small numbers for vec3i, see hash_analysis.py
	//TODO: this will fail for vec3f since shifting is not defined for floats, so type specify this function
	template <typename T> struct hash<vec3Templated<T>>{
		#ifndef ROTATE_LEFT
		#define ROTATE_LEFT(x, amount) ((x << amount) | (x >> (sizeof(x) * 8 - amount)))
		#endif

		std::size_t operator()(const vec3Templated<T>& k) const{
			//x = 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00 
			//y = 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00 31 30 29 28 27 26 25 24 23 22 
			//z = 10 09 08 07 06 05 04 03 02 01 00 31 30 29 28 27 26 25 24 23 22 31 20 19 18 17 16 15 14 13 12 11 
			using std::hash;
			return hash<T>()((k.x) ^ ROTATE_LEFT(k.y, 10) ^ ROTATE_LEFT(k.z, 21));
		}
	};
}
#endif
