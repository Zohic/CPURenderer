#pragma once
#include "Transform.h"

namespace cpuRenderBase {
	class Camera {
		Mat4x4 projectionMatrix;
		enum class CameraType {
			ORTHOGRAPHIC,
			PERSPECTIVE
		};
	public:
		Transform transform;

		Mat4x4 GetViewMatrix() const;
		Mat4x4 GetProjectionMatrix() const;

		void SetFrustum(float fovY, float aspectRatio, float front, float back);
	};
}