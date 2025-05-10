#include "MeshData.h"
using namespace cpuRenderBase;

MeshData::MeshData() : vertices(), normal(), tangent(), texcoord(), color(), indices(), triangleCount(0) {

}

MeshData::~MeshData() {

}

MeshData::MeshData(MeshData&& old) noexcept : AttributeAvailability(static_cast<AttributeAvailability&&>(old)),
vertices(std::move(old.vertices)),
normal(std::move(old.normal)),
tangent(std::move(old.tangent)),
texcoord(std::move(old.texcoord)),
color(std::move(old.color)),
indices(std::move(old.indices)),
triangleCount(old.triangleCount) {

	attributeList = old.attributeList;
	attributeMask = old.attributeMask;

	old.triangleCount = 0;
}

MeshData& MeshData::operator=(MeshData&& old) noexcept {
	vertices = std::move(old.vertices);
	normal = std::move(old.normal);
	tangent = std::move(old.tangent);
	texcoord = std::move(old.texcoord);
	color = std::move(old.color);
	indices = std::move(old.indices);

	triangleCount = old.triangleCount;
	old.triangleCount = 0;

	attributeList = old.attributeList;
	attributeMask = old.attributeMask;

	return *this;
}

//this works because Vec2, Vec3, Vec4 classes have only one field as array of floats

//void MeshData::SetAttr(size_t ind, std::vector<float>&& list) {
//	if (ind > ATTR_COLOR_INDEX) {
//		throw std::logic_error("invalid list ind");
//	}
//
//	const char* const offsetByAttributes = ((char*)this) + offsetof(MeshData, vertices);
//	std::vector<float>* const firstList = (std::vector<float>*const)(offsetByAttributes);
//
//	*(firstList + ind) = std::move(list);
//}


void MeshData::SetIndices(std::vector<uint32_t>&& list) {
	indices = list;
	CalcTriangleCount();
}
void MeshData::PrintData() const {
	for (int i = 0; i < vertices.size(); i += 1) {
		printf("vertex %i: (%.6f, %.6f, %.6f)\n", i + 1, vertices[i][0], vertices[i][1], vertices[i][2]);
	}
	for (int i = 0; i < normal.size(); i += 1) {
		printf("normal %i: (%.6f, %.6f, %.6f)\n", i + 1, normal[i][0], normal[i][1], normal[i][2]);
	}
	for (int i = 0; i < texcoord.size(); i += 1) {
		printf("texcoord %i: (%f, %f)\n", i + 1, texcoord[i][0], texcoord[i][1]);
	}
	for (int i = 0; i < color.size(); i += 1) {
		printf("color %i: (%.6f, %.6f, %.6f, %.6f)\n", i + 1, color[i][0], color[i][1], color[i][2], color[i][3]);
	}

	for (int i = 0; i < indices.size(); i += 3) {
		printf("index %i: (%i, %i, %i)\n", i / 3 + 1, indices[i], indices[i + 1], indices[i + 2]);
	}

	printf("\t%i triangles in total\n", triangleCount);
}

void MeshData::CalcTriangleCount() {
	triangleCount = indices.size() / 3;
}

const Vec3& MeshData::GetPos(size_t ind) const {
	using namespace std::string_literals;
	if (ind > indices.size())
		throw std::logic_error(("Getting pos attribute out of bounds "s + ind + " amount of vertices is "s + indices.size()).c_str());
	return vertices[indices[ind]];
}
const Vec3& MeshData::GetNormal(size_t ind) const {
	if (ind > indices.size())
		throw std::logic_error("Getting normal attribute out of bounds");
	return normal[indices[ind]];
}
const Vec4& MeshData::GetTangent(size_t ind) const {
	if (ind > indices.size())
		throw std::logic_error("Getting tangent attribute out of bounds");
	return tangent[indices[ind]];
}
const Vec2& MeshData::GetTexcoord(size_t ind) const {
	if (ind > indices.size())
		throw std::logic_error("Getting texcoord attribute out of bounds");
	return texcoord[indices[ind]];
}
const Vec4& MeshData::GetColor(size_t ind) const {
	if (ind > indices.size())
		throw std::logic_error("Getting color attribute out of bounds");
	return color[indices[ind]];
}

size_t MeshData::GetVertexCount() const {
	return vertices.size();
}
uint32_t MeshData::GetTriangleCount() const {
	return triangleCount;
}