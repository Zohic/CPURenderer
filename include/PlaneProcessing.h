#include "TriangleWrap.h"

namespace cpuRenderBase {
	namespace utils {
		//lineStart will be exchanged with interpolated value
		void InterpolateToPlane(const Vec4& plane, VertexData& lineStart, const VertexData& lineEnd, const bool* attrList);

		//note that lineStart will be interpolated to the plane even if its on the right side of it
		int ClipAgainstPlane(const Vec4& plane, int clipped_ind, std::vector<TriangleWrap>& to_append, const bool* attrList);
		
	}
}

