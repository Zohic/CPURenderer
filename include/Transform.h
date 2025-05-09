#pragma once
#include "CPURENDefines.h"


namespace cpuRenderBase {

	namespace utils {
		inline void FillRotationMatrix(const Vec3& angles, Mat4x4& outMat) {
			float sx = sin(angles.x());
			float cx = cos(angles.x());

			// rotation angle about Y-axis (yaw)
			float sy = sin(angles.y());
			float cy = cos(angles.y());

			// rotation angle about Z-axis (roll)
			float sz = sin(angles.z());
			float cz = cos(angles.z());

			// determine left axis
			outMat(0, 0) = cy * cz;
			outMat(1, 0) = sx * sy * cz + cx * sz;
			outMat(2, 0) = -cx * sy * cz + sx * sz;

			// determine up axis
			outMat(0, 1) = -cy * sz;
			outMat(1, 1) = -sx * sy * sz + cx * cz;
			outMat(2, 1) = cx * sy * sz + sx * cz;

			// determine forward axis
			outMat(0, 2) = sy;
			outMat(1, 2) = -sx * cy;
			outMat(2, 2) = cx * cy;

			outMat(3, 0) = 0;
			outMat(3, 1) = 0;
			outMat(3, 2) = 0;
			outMat(3, 3) = 1;

			outMat(0, 3) = 0;
			outMat(1, 3) = 0;
			outMat(2, 3) = 0;
		}
	}
	
	class Transform
	{
	private:
		Transform* parent = nullptr;
	public:
		Vec3 pos, rot, scale;
		Transform();
		Transform(Vec3 p);
		Transform(Vec3 p, Vec3 r);
		Transform(Vec3 p, Vec3 r, Vec3 s);

		void SetParent(Transform* const par);

		Mat4x4 GetTranslationMatrix() const;

		Mat4x4 GetScaleMatrix() const;

		Mat4x4 GetRotationMatrix() const;

		void TransformWorldMatrix(Mat4x4& res) const;

		Mat4x4 GetWorldMatrix() const;
	};
}

