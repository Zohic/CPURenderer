#pragma once
#include <vector>

#include "CPURENDefines.h"
#include "AttributeAvailability.h"

namespace cpuRenderBase {

	using Vec2 = gmtl::Vec2f;
	using Vec3 = gmtl::Vec3f;
	using Vec4 = gmtl::Vec4f;
	using Mat4x4 = gmtl::Matrix44f;

	class MeshData final : public AttributeAvailability {
		std::vector<Vec3> vertices;
		std::vector<Vec3> normal;
		std::vector<Vec4> tangent;
		std::vector<Vec2> texcoord;
		std::vector<Vec4> color;

		std::vector<uint32_t> indices;

		uint32_t triangleCount;

		MeshData(const MeshData& other) = delete;
		MeshData& operator=(const MeshData& other) = delete;

	public:
		MeshData();
		virtual ~MeshData();

		MeshData(MeshData&& old) noexcept;
		MeshData& operator=(MeshData&& old) noexcept;


		void SetAttr(size_t ind, std::vector<float>&& list);

		void SetIndices(std::vector<uint32_t>&& list);
		void PrintData() const;

		void CalcTriangleCount();

		const Vec3& GetPos(size_t ind) const;
		const Vec3& GetNormal(size_t ind) const;
		const Vec4& GetTangent(size_t ind) const;
		const Vec2& GetTexcoord(size_t ind) const;
		const Vec4& GetColor(size_t ind) const;

		size_t GetVertexCount() const;
		uint32_t GetTriangleCount() const;
	};

}