#include "Mesh.h"

using namespace cpuRenderBase;

Mesh::Mesh() {

}

//reserves
void Mesh::ReserveMeshCount(size_t c) {
	meshes.reserve(c);
}
void Mesh::AddMesh(MeshData&& m) {
	meshes.push_back(std::move(m));
}
const std::vector<MeshData>& Mesh::GetMeshData() const {
	/*if (ind >= meshes.size())
		throw std::logic_error("trying to get mesh with index out of bounds");

	printf("meshRet %zu\n", meshes.size());*/
	return meshes;
}
uint8_t Mesh::GetCombinedAtrrMask() const {
	uint8_t mask = UINT8_MAX;

	for (const auto& md : meshes)
		mask &= md.GetAttrMask();

	return mask;
}
void Mesh::ResetIterators() {
	meshesIter = meshes.cbegin();
	vertIter = 0;
}
bool Mesh::End() const {
	printf("huh\n");

	bool a = meshes.cend() == meshesIter;

	printf("huh\n");
	return a;
}
void Mesh::PrintData() {
	for (size_t i = 0; i < meshes.size(); i++) {
		printf("mesh %zu: \n", i);
		meshes[i].PrintData();
	}
}