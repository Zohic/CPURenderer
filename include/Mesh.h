#pragma once
#include "MeshData.h"

namespace cpuRenderBase {
	class Mesh {
		std::vector<MeshData> meshes;
		mutable std::vector<MeshData>::const_iterator meshesIter;
		mutable size_t vertIter = 0;

	public:

		Mesh();
		Mesh(MeshData&& m);
		Mesh(std::vector<MeshData>&& meshes);
		
		Mesh(const Mesh& m) = delete;
		Mesh(Mesh&& m);

		Mesh& operator=(const Mesh& m) = delete;
		Mesh& operator=(Mesh&& m);


		//reserves
		void ReserveMeshCount(size_t c);

		void AddMesh(MeshData&& m);

		const std::vector<MeshData>& GetMeshData() const;

		uint8_t GetCombinedAtrrMask() const;

		void ResetIterators();

		bool End() const;

		void PrintData();
	};
}