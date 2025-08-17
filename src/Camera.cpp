#include "Camera.h"

using namespace cpuRenderBase;

Mat4x4 Camera::GetViewMatrix() const {
	return Mat4x4{};
}
const Mat4x4& Camera::GetProjectionMatrix() const {
	return projectionMatrix;
}

// aspectRatio is H/W
void Camera::SetFrustum(float fovY, float aspectRatio, float near, float far)
{
	DEBUGPRINT("SETTING FRUSTUM\n");
	const float DEG2RAD = PI / 180.0f;

	const float tangent = tan(fovY / 2 * DEG2RAD);    
	const float fmn = far - near;
	// params: left, right, bottom, top, near(front), far(back)
	projectionMatrix.setZero();
	projectionMatrix(0, 0) = 1.0f / tangent;
	projectionMatrix(1, 1) = 1.0f / tangent / aspectRatio;
	projectionMatrix(2, 2) = far / fmn;
	projectionMatrix(2, 3) = -(far * near) / fmn;
	projectionMatrix(3, 2) = 1;
	projectionMatrix(3, 3) = 0;
}
