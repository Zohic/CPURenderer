#include <memory>
#include "VertexData.h"

using namespace cpuRenderBase;

inline size_t VertexData::GetAttrFloatSize(const bool* attrMask) {
	if (attrMask == nullptr)
		throw std::logic_error("attribute mask is nullptr when GetAttrFloatSize() \n");

	return
		4ULL +
		3ULL * static_cast<unsigned int>(attrMask[ATTR_NORMAL_INDEX]) +
		4ULL * static_cast<unsigned int>(attrMask[ATTR_TANGENT_INDEX]) +
		2ULL * static_cast<unsigned int>(attrMask[ATTR_TEXCOORD_INDEX]) +
		4ULL * static_cast<unsigned int>(attrMask[ATTR_COLOR_INDEX]);
}

VertexData::VertexData() : attributes(nullptr) {

}

VertexData VertexData::Copy(const bool* attrList) const {
	VertexData copy;
	size_t attrSize = GetAttrFloatSize(attrList);
	copy.attributes = new float[attrSize];
	memcpy(copy.attributes, this->attributes, attrSize * sizeof(float));

#ifdef CPUREN_DEBUG
	copy.size = size;
#endif
	//copy.attributes.reset(new float[attrSize]);
	//memcpy(copy.attributes, this->attributes, attrSize * sizeof(float));

	return copy;
}

VertexData::VertexData(VertexData&& old) noexcept {
	this->attributes = old.attributes;
	old.attributes = nullptr;
#ifdef _DEBUG
	this->_attrList = old._attrList;
	old._attrList = nullptr;
#endif
	//attributes = std::move(old.attributes);
}
VertexData& VertexData::operator=(VertexData&& old) noexcept {
	this->attributes = old.attributes;
	old.attributes = nullptr;
#ifdef _DEBUG
	this->_attrList = old._attrList;
	old._attrList = nullptr;
#endif
	//attributes = std::move(old.attributes);
	return *this;
}

void VertexData::Init(const bool* attrList) {
	if (attributes != nullptr) {
		throw std::logic_error("trying to Init already occupied vertex data");
	}
	const size_t attrSize = GetAttrFloatSize(attrList);
	attributes = new float[attrSize];
	memset(attributes, 0, attrSize * sizeof(float));

#ifdef _DEBUG
	_attrList = new bool[ATTRIBUTES_NUM];
	for(int i=0;i<5;i++)
		_attrList[i]= attrList[i];
#endif

	//DEBUGPRINT("size is set to %i: \n", attrSize);
	//attributes.reset(new float[attrSize]);
	//memset(attributes, 0, attrSize);
}

void VertexData::SetFromMesh(const MeshData* const mesh, const bool* reqList, size_t vertInd) {
	//attributes.reset(new float[GetAttrFloatSize(attrReqList)]);

	if (mesh == nullptr)
		throw std::logic_error("mesh is nullptr when setting vertex data from mesh \n");

	Init(reqList);

	SetPos(mesh->GetPos(vertInd));

	if (mesh->HasAttr(ATTR_NORMAL_INDEX) && reqList[ATTR_NORMAL_INDEX])
		SetNormal(mesh->GetNormal(vertInd));

	if (mesh->HasAttr(ATTR_TANGENT_INDEX) && reqList[ATTR_TANGENT_INDEX])
		SetTangent(mesh->GetTangent(vertInd), reqList);

	if (mesh->HasAttr(ATTR_TEXCOORD_INDEX) && reqList[ATTR_TEXCOORD_INDEX])
		SetTexcoord(mesh->GetTexcoord(vertInd), reqList);

	if (mesh->HasAttr(ATTR_COLOR_INDEX) && reqList[ATTR_COLOR_INDEX])
		SetColor(mesh->GetColor(vertInd), reqList);

}


void VertexData::SetPos(const Vec3& pos) {
	if (!attributes)
		throw std::logic_error("cannot SetPos(): vertex data is null\n");
	attributes[0] = pos.x();
	attributes[1] = pos.y();
	attributes[2] = pos.z();
	attributes[3] = 1.0f;
}
void VertexData::SetPos(const Vec4& pos) {
	if (!attributes)
		throw std::logic_error("cannot SetPos(): vertex data is null\n");
	attributes[0] = pos.x();
	attributes[1] = pos.y();
	attributes[2] = pos.z();
	attributes[3] = pos.w();
}
void VertexData::SetNormal(const Vec3& norm) {
	if (!attributes)
		throw std::logic_error("cannot SetNormal(): vertex data is null\n");
	attributes[4] = norm.x();
	attributes[5] = norm.y();
	attributes[6] = norm.z();
}
void VertexData::SetTangent(const Vec4& tang, const bool* attrMask) {
	if (!attributes)
		throw std::logic_error("cannot SetTangent(): vertex data is null\n");
	const int offset =
		4 +
		3 * static_cast<unsigned int>(attrMask[ATTR_NORMAL_INDEX]);
	attributes[offset + 0] = tang.x();
	attributes[offset + 1] = tang.y();
	attributes[offset + 2] = tang.z();
	attributes[offset + 3] = tang.w();
}
void VertexData::SetTexcoord(const Vec2& texcrd, const bool* attrMask) {
	if (!attributes)
		throw std::logic_error("cannot SetTexcoord(): vertex data is null\n");
	const int offset =
		4 +
		3 * static_cast<unsigned int>(attrMask[ATTR_NORMAL_INDEX]) +
		4 * static_cast<unsigned int>(attrMask[ATTR_TANGENT_INDEX]);

	attributes[offset + 0] = texcrd.x();
	attributes[offset + 1] = texcrd.y();
}
void VertexData::SetColor(const Vec4& clr, const bool* attrMask) {
	if (!attributes)
		throw std::logic_error("cannot SetColor(): vertex data is null\n");
	const int offset =
		4 +
		3 * static_cast<unsigned int>(attrMask[ATTR_NORMAL_INDEX]) +
		4 * static_cast<unsigned int>(attrMask[ATTR_TANGENT_INDEX]) +
		2 * static_cast<unsigned int>(attrMask[ATTR_TEXCOORD_INDEX]);

	attributes[offset + 0] = clr.x();
	attributes[offset + 1] = clr.y();
	attributes[offset + 2] = clr.z();
	attributes[offset + 3] = clr.w();
}

const Vec4& VertexData::GetPos() const {
	if (!attributes)
		throw std::logic_error("cannot GetPos(): vertex data is null\n");
	return *((Vec4*)attributes);
	//return Vec4(attributes[0], attributes[1], attributes[2], attributes[3]);
}
const Vec3& VertexData::GetNormal() const {
	if (!attributes)
		throw std::logic_error("cannot GetNormal(): vertex data is null\n");
	const float* const nums = (attributes + 4);
	return *((Vec3*)nums);
}
const Vec4& VertexData::GetTangent(const bool* attrMask) const {
	if (!attributes)
		throw std::logic_error("cannot GetTangent(): vertex data is null\n");

	const float* const nums = (attributes
		+ 4u
		+ 3u * static_cast<unsigned int>(attrMask[ATTR_NORMAL_INDEX]));

	return *((Vec4*)nums);
}
const Vec2& VertexData::GetTexcoord(const bool* attrMask) const {
	if (!attributes)
		throw std::logic_error("cannot GetTexcoord(): vertex data is null\n");
	const float* const nums = (attributes
		+ 4u
		+ 3u * static_cast<unsigned int>(attrMask[ATTR_NORMAL_INDEX])
		+ 4u * static_cast<unsigned int>(attrMask[ATTR_TANGENT_INDEX]));

	return *((Vec2*)nums);
}
const Vec4& VertexData::GetColor(const bool* attrMask) const {
	if (!attributes)
		throw std::logic_error("cannot GetColor(): vertex data is null\n");
	const float* const nums = (attributes
		+ 4u
		+ 3u * static_cast<unsigned int>(attrMask[ATTR_NORMAL_INDEX])
		+ 4u * static_cast<unsigned int>(attrMask[ATTR_TANGENT_INDEX])
		+ 2u * static_cast<unsigned int>(attrMask[ATTR_TEXCOORD_INDEX]));

	return *((Vec4*)nums);
}

void VertexData::PrintPos() const {
	DEBUGPRINT("\tpos is: (%f, %f, %f, %f)\n", GetPos().x(), GetPos().y(), GetPos().z(), GetPos().w());
}
void VertexData::PrintNormal() const {
	DEBUGPRINT("\tnormal is: (%f, %f, %f)\n", GetNormal().x(), GetNormal().y(), GetNormal().z());
}
void VertexData::PrintColor(const bool* attrMask) const {
	const Vec4& clr = GetColor(attrMask);
	DEBUGPRINT("\tcolor is: (%f, %f, %f, %f)\n", clr.x(), clr.y(), clr.z(), clr.w());
}