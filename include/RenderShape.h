#pragma once
#include "Mesh.h"
#include "Material.h"

namespace cpuRenderBase {
	class RenderShape {
	private:
		const Mesh* mesh = nullptr;
		const Material* material = nullptr;
		std::vector<VertexData> prepData;

		RenderShape() = delete;

		mutable size_t localVertInd_ = 0;
		size_t meshDataInd = 0;

	public:
		RenderShape(const Mesh* imesh, const Material* imat);
		void SetMesh(const Mesh* imesh);
		void SetMaterial(const Material* mat);

		void SetUniform();

		const Mesh& GetMesh() const;
		const Material& GetMaterial() const;

		void ResetIterator() const;

		bool EndOfTriangles() const;

		void GetTrinagle(VertexData* const verts) const;


	};
}
