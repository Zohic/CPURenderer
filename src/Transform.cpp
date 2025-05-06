#include "Transform.h"
using namespace cpuRenderBase;

Transform::Transform() : pos(), rot(), scale(1.0f, 1.0f, 1.0f)
{

}
Transform::Transform(Vec3 p) : pos(p), rot(), scale(1.0f, 1.0f, 1.0f)
{

}
Transform::Transform(Vec3 p, Vec3 r) : pos(p), rot(r), scale(1.0f, 1.0f, 1.0f)
{

}
Transform::Transform(Vec3 p, Vec3 r, Vec3 s) : pos(p), rot(r), scale(s)
{

}

void Transform::SetParent(Transform* const par) {
	if (par == nullptr)
		throw std::logic_error("setting nullptr as a parent for the transform");
	parent = par;
}

Mat4x4 Transform::GetTranslationMatrix() const {
	Mat4x4 mTrans;
	mTrans[0][3] = pos.x();
	mTrans[1][3] = pos.y();
	mTrans[2][3] = pos.z();
	mTrans[3][3] = 1.0f;

	return mTrans;
}

Mat4x4 Transform::GetScaleMatrix() const {
	Mat4x4 mScale;

	mScale[0][0] = scale.x();
	mScale[1][1] = scale.y();
	mScale[2][2] = scale.z();
	mScale[3][3] = 1.0f;

	return mScale;
}

Mat4x4 Transform::GetRotationMatrix() const {

	Mat4x4 mRot;
	utils::FillRotationMatrix(rot, mRot);
	return mRot;
}

void Transform::TransformWorldMatrix(Mat4x4& res) const {
	res = GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix() * res;
}

Mat4x4 Transform::GetWorldMatrix() const {
	Mat4x4 resultMat;
	Transform* trans = parent;

	TransformWorldMatrix(resultMat);

	while (trans != nullptr) {
		trans->TransformWorldMatrix(resultMat);
		trans = trans->parent;
	}

	return resultMat;
}
