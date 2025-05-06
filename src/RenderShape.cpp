#include "RenderShape.h"
using namespace cpuRenderBase;

RenderShape::RenderShape(const Mesh* imesh, const Material* imat) : mesh(imesh), material(imat) {
	size_t triagCount = 0;
	for (const auto& meshData : mesh->GetMeshData()) {
		triagCount += meshData.GetTriangleCount();
	}

	prepData.resize(triagCount * 3);

	printf("triangles reserved %zu\n", triagCount);

	size_t prepInd = 0;
	for (const auto& meshData : mesh->GetMeshData()) {
		printf("next mesh, %i triangles\n", meshData.GetTriangleCount());
		for (size_t vertInd = 0; vertInd < meshData.GetTriangleCount() * 3ULL; vertInd += 3ULL) {
			//printf("prep ind %zu vert ind %zu\n", prepInd, vertInd);
			prepData[prepInd + 0ULL].SetFromMesh(&meshData, material->GetAttrList(), vertInd + 0);
			prepData[prepInd + 1ULL].SetFromMesh(&meshData, material->GetAttrList(), vertInd + 1);
			prepData[prepInd + 2ULL].SetFromMesh(&meshData, material->GetAttrList(), vertInd + 2);

			prepInd += 3ULL;
		}
	}
	printf("shape is made\n");
}

void RenderShape::SetMesh(const Mesh* imesh) {
	mesh = imesh;
}

void RenderShape::SetMaterial(const Material* mat) {
	const uint8_t& req = mat->GetAttrMask();
	const uint8_t& avl = mesh->GetCombinedAtrrMask();

	bool hasAttr = (req & avl) == req;

	material = mat;

	if (!hasAttr)
		printf("WARNING WHEN CREATING SHAPE: the material reqires more attributes than the mesh has\n");
}

void RenderShape::SetUniform() {

}

const Mesh& RenderShape::GetMesh() const {
	if (!mesh)
		throw std::logic_error("trying to access mesh from shape that is not existent");
	return *mesh;
}
const Material& RenderShape::GetMaterial() const {
	if (!material)
		throw std::logic_error("trying to access material from shape that is not existent");
	return *material;
}

void RenderShape::ResetIterator() const {
	localVertInd_ = 0;
}

bool RenderShape::EndOfTriangles() const {
	return localVertInd_ >= prepData.size();
}

void RenderShape::GetTrinagle(VertexData* const verts) const {
	DEBUGPRINT("shape returning vertices: (%zu/%zu)\n", localVertInd_, prepData.size());

	verts[0] = prepData[localVertInd_ + 0].Copy(material->GetAttrList());
	verts[1] = prepData[localVertInd_ + 1].Copy(material->GetAttrList());
	verts[2] = prepData[localVertInd_ + 2].Copy(material->GetAttrList());

	localVertInd_ += 3;
}