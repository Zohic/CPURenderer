#pragma once
#include <vector>

#include "CPURENDefines.h"
#include "AttributeAvailability.h"
#include <iostream>


namespace cpuRenderBase {

	constexpr size_t ATTR_VEC_SIZE[] = { 3, 3, 4, 2, 4 };

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


		template<size_t attr_ind>
		void SetAttr(std::vector<float>&& list)
		{
			if (list.size() % ATTR_VEC_SIZE[attr_ind] != 0)
				throw std::logic_error("size must be divisible by");

			const char* const offsetByAttributes = ((char*)this) + offsetof(MeshData, vertices);
			std::vector<float>* const firstList = (std::vector<float>*const)(offsetByAttributes);
			
			*(firstList + attr_ind) = std::move(list);
		}


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