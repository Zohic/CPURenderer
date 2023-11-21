
#include <iostream>

namespace vecmath {
	const float PI = 3.14159265f;
	const float PI_2 = PI * 0.5f;
	const float PI2 = 3.141592f * 2.0f;

	const float EPS2 = std::numeric_limits<float>::epsilon() * 2;

	struct Vec2
	{
		float x, y;

		Vec2()
		{
			x = y = 0;
		}
		Vec2(float ix, float iy): x(ix), y(iy)
		{

		}
		Vec2(const Vec2& v2): Vec2(v2.x, v2.y)
		{

		}

		Vec2 operator =(const Vec2& v2)
		{
			x = v2.x;
			y = v2.y;
			return *this;
		}
		Vec2 operator +(const Vec2& v2) const
		{
			return Vec2(this->x + v2.x, this->y + v2.y);
		}
		Vec2& operator +=(const Vec2& v2)
		{
			this->x += v2.x;
			this->y += v2.y;
			return *this;
		}
		Vec2 operator -(const Vec2& v2) const
		{
			return Vec2(this->x - v2.x, this->y - v2.y);
		}
		Vec2& operator -=(const Vec2& v2)
		{
			this->x -= v2.x;
			this->y -= v2.y;
			return *this;
		}
		Vec2 operator *(float k)const 
		{
			return Vec2(this->x * k, this->y * k);
		}
		Vec2& operator *=(float k)
		{
			this->x *= k;
			this->y *= k;
			return *this;
		}
	};

	struct Vec3
	{
		float x, y, z;

		Vec3() : x(0), y(0), z(0)
		{

		}
		Vec3(float c) : x(c), y(c), z(c)
		{
		}
		Vec3(float ix, float iy, float iz) : x(ix), y(iy), z(iz)
		{
		}
		Vec3(const Vec3& other): Vec3(other.x, other.y, other.z)
		{
			
		}
		
		Vec3& operator =(const Vec3& v2)
		{
			x = v2.x;
			y = v2.y;
			z = v2.z;
			return *this;
		}
		Vec3 operator +(const Vec3& v2) const
		{
			return Vec3(this->x + v2.x, this->y + v2.y, this->z + v2.z);
		}
		Vec3& operator +=(const Vec3& v2)
		{
			this->x += v2.x;
			this->y += v2.y;
			this->z += v2.z;
			return *this;
		}
		Vec3 operator -(const Vec3& v2) const
		{
			return Vec3(this->x - v2.x, this->y - v2.y, this->z - v2.z);
		}
		Vec3& operator -=(const Vec3& v2)
		{
			this->x -= v2.x;
			this->y -= v2.y;
			this->z -= v2.z;
			return *this;
		}
		Vec3 operator *(float k) const
		{
			return Vec3(this->x * k, this->y * k, this->z * k);
		}
		Vec3& operator *=(float k)
		{
			this->x *= k;
			this->y *= k;
			this->z *= k;
			return *this;
		}
		Vec3 operator /(float k) const
		{
			if (k <= EPS2)
				return Vec3(0);
			return Vec3(x/k, y/k, z/k);
		}
		Vec3& operator /=(float k)
		{
			if (k <= EPS2)
				Vec3(0);
			else
			{
				this->x /= k;
				this->y /= k;
				this->z /= k;
			}
			return *this;
		}
		void MultiplyPerComponent(const Vec3& coefs)
		{
			x *= coefs.x;
			y *= coefs.y;
			z *= coefs.z;
		}
	};



	//returns point rotated around gloabl X axis (1, 0, 0)
	Vec3 rotateX(const Vec3& in, float ang)
	{
		float sn = sinf(ang);
		float cs = cosf(ang);
		return Vec3(in.x, in.y * cs - in.z * sn , in.z * cs + in.y * sn );
	}
	//returns point rotated around gloabl Y axis (0, 1, 0)
	Vec3 rotateY(const Vec3& in, float ang)
	{
		float sn = sinf(ang);
		float cs = cosf(ang);
		return Vec3(in.x * cs - in.z * sn, in.y, in.z * cs + in.x * sn);
	}
	//returns point rotated around gloabl Z axis (0, 0, 1)
	Vec3 rotateZ(Vec3 in, float ang)
	{
		float sn = sinf(ang);
		float cs = cosf(ang);
		return Vec3(in.x * cs - in.y * sn, in.y * cs + in.x * sn , in.z);
	}
	//returns point rotated sequentially around gloabl X, Y, Z axes in specified order
	Vec3 rotate3d(Vec3 in, Vec3 angles, int order)
	{
		switch (order)
		{
		case 0:
			in = rotateX(in, angles.x);
			in = rotateY(in, angles.y);
			in = rotateZ(in, angles.z);
			break;
		case 1:
			in = rotateX(in, angles.x);
			in = rotateZ(in, angles.z);
			in = rotateY(in, angles.y);
			break;
		case 2:
			in = rotateY(in, angles.y);
			in = rotateX(in, angles.x);
			in = rotateZ(in, angles.z);
			break;
		case 3:
			in = rotateZ(in, angles.z);
			in = rotateX(in, angles.x);
			in = rotateY(in, angles.y);
			break;
		case 4:
			in = rotateY(in, angles.y);
			in = rotateZ(in, angles.z);
			in = rotateX(in, angles.x);
			break;
		case 5:
			in = rotateZ(in, angles.z);
			in = rotateY(in, angles.y);
			in = rotateX(in, angles.x);
			break;

		default:
			in = rotateY(in, angles.y);
			in = rotateX(in, angles.x);
			in = rotateZ(in, angles.z);
			break;
		}

		return in;
	}

	

	float Length(const Vec3& in)
	{
		float s = in.x * in.x + in.y * in.y + in.z * in.z;
		if (s > 0)
			return sqrtf(s);
		else return 0;
	}
	float DotProduct(const Vec3& v1, const Vec3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	float Distance(const Vec3& p1, const Vec3& p2)
	{
		return Length(p1 - p2);
	}
	Vec3 CrossProduct(const Vec3& v1, const Vec3& v2)
	{
		return Vec3(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
	}
	Vec3 NormaliseVec(const Vec3& vec)
	{
		return vec / Length(vec);
	}
	Vec3 Reflect(const Vec3& vec, const Vec3& norm)
	{
		//        r = v - norm * 2 * dp(v,norm)
		return vec - norm * (2.0f * DotProduct(vec, norm));
	}

	/*Pnt3 Vector_IntersectPlane(Pnt3 &plane_p, Pnt3 &plane_n, Pnt3 &lineStart, Pnt3 &lineEnd)
	{
		plane_n = NormaliseVec(plane_n);
		float plane_d = -DotProduct(plane_n, plane_p);
		float ad = DotProduct(lineStart, plane_n);
		float bd = DotProduct(lineEnd, plane_n);
		float t = (-plane_d - ad) / (bd - ad);
		Pnt3 lineStartToEnd = lineStartToEnd.dis(lineEnd, lineStart);
		Pnt3 lineToIntersect = lineToIntersect.mul(lineStartToEnd, t);
		return lineStartToEnd.sum(lineStart, lineToIntersect);
	}*/

	struct Quaternion
	{
		float x, y, z, w;

		void FromEuler(Vec3 euler)// for ZYX sequence
		{
			double cr = cos(euler.x * 0.5);
			double sr = sin(euler.x * 0.5);
			double cp = cos(euler.y * 0.5);
			double sp = sin(euler.y * 0.5);
			double cy = cos(euler.z * 0.5);
			double sy = sin(euler.z * 0.5);

			w = cr * cp * cy + sr * sp * sy;
			x = sr * cp * cy - cr * sp * sy;
			y = cr * sp * cy + sr * cp * sy;
			z = cr * cp * sy - sr * sp * cy;
		}

		Vec3 toEuler()
		{
			Vec3 angles;

			// roll (x-axis rotation)
			float sinr_cosp = 2 * (w * x + y * z);
			float cosr_cosp = 1 - 2 * (x * x + y * y);
			angles.x = std::atan2(sinr_cosp, cosr_cosp);

			// pitch (y-axis rotation)
			float sinp = std::sqrt(1 + 2 * (w * y - x * z));
			float cosp = std::sqrt(1 - 2 * (w * y - x * z));
			angles.y = 2 * std::atan2(sinp, cosp) - PI / 2;

			// yaw (z-axis rotation)
			float siny_cosp = 2 * (w * z + x * y);
			float cosy_cosp = 1 - 2 * (y * y + z * z);
			angles.z = std::atan2(siny_cosp, cosy_cosp);

			return angles;
		}

		void FromAxisAngle(Vec3 axis, float radians) {
			float sinAngle;

			radians *= 0.5f;
			axis = NormaliseVec(axis);
			sinAngle = sin(radians);
			x = axis.x * sinAngle;
			y = axis.y * sinAngle;
			z = axis.z * sinAngle;
			w = cos(radians);
		}

		void Multiply(Quaternion rhs) {

			x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
			y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
			z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;
			w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
		}

		void Rotate(Vec3 axis, float radians) {
			Quaternion rotationQuaternion;
			rotationQuaternion.FromAxisAngle(axis, radians);
			Multiply(rotationQuaternion);
		}
	};
}

