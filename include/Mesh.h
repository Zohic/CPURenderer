#pragma once
#include "MeshData.h"

namespace cpuRenderBase {
	class Mesh {
		std::vector<MeshData> meshes;
		mutable std::vector<MeshData>::const_iterator meshesIter;
		mutable size_t vertIter = 0;
	public:

		Mesh();

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