#pragma once

#include "MeshData.h"

namespace cpuRenderBase {
	class VertexData {
			float* attributes;
	#ifdef _DEBUG
			bool* _attrList;
	#endif // _DEBUG

			inline static size_t GetAttrFloatSize(const bool* attrMask);

			VertexData(const VertexData& other) = delete;
			VertexData& operator=(const VertexData& other) = delete;

		public:
			VertexData();
			~VertexData() {
				if (attributes != nullptr)
					delete[] attributes;
				//else throw std::logic_error("there was VertexData object with nullptr attributes");
	#ifdef _DEBUG
				if(_attrList != nullptr)
					delete[] _attrList;
	#endif
			}
			VertexData Copy(const bool* attrList) const;

			VertexData(VertexData&& old) noexcept;
			VertexData& operator=(VertexData&& old) noexcept;

			void Init(const bool* attrList);

			void SetFromMesh(const MeshData* const mesh, const bool* reqList, size_t vertInd);

			void SetPos(const Vec3& pos);
			void SetPos(const Vec4& pos);
			void SetNormal(const Vec3& norm);
			void SetTangent(const Vec4& tang, const bool* attrMask);
			void SetTexcoord(const Vec2& texcrd, const bool* attrMask);
			void SetColor(const Vec4& clr, const bool* attrMask);

			const Vec4& GetPos() const;
			const Vec3& GetNormal() const;
			const Vec4& GetTangent(const bool* attrMask) const;
			const Vec2& GetTexcoord(const bool* attrMask) const;
			const Vec4& GetColor(const bool* attrMask) const;

			void PrintPos() const;
			void PrintNormal() const;
			void PrintColor(const bool* attrMask) const;
	#ifdef CPUREN_DEBUG
			float operator[](const int ind) const {
				if (ind < size)
					return attributes[ind];
				else
					return 0.0f;
			}
	#endif // CPUREN_DEBUG

	};
}
