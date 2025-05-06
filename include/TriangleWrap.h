#pragma once

#include "VertexData.h"

namespace cpuRenderBase {
	struct TriangleWrap {
		explicit TriangleWrap() = delete;
		explicit TriangleWrap(VertexData* v1);
		explicit TriangleWrap(const VertexData* v1);

		TriangleWrap(const TriangleWrap& other) = delete;
		TriangleWrap& operator=(const TriangleWrap& other) = delete;

		TriangleWrap(TriangleWrap&& old) noexcept;
		TriangleWrap& operator=(TriangleWrap&& old) noexcept;

		bool operator ==(const TriangleWrap& other) const;

		VertexData& operator[](const size_t n);
		const VertexData& operator[](size_t n) const;

		static TriangleWrap NewTriag() {
			return TriangleWrap(new VertexData[3]);
		};

		void Free() {
			_vert = nullptr;
		}

		~TriangleWrap() noexcept(false);
	private:
		VertexData* _vert;
	};

}