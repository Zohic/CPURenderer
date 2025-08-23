#include "PlaneProcessing.h"

#define V4asV3(v4) (*(reinterpret_cast<Vec3*>(&v4)))
#define V4asV3_Const(cv4) (*(reinterpret_cast<const Vec3*>(&cv4)))

namespace cpuRenderBase {
	namespace utils {
		void InterpolateToPlane(const Vec4& plane, VertexData& lineStart, const VertexData& lineEnd, const bool* attrList) {

#define v1 V4asV3_Const(lineStart.GetPos())
#define v2 V4asV3_Const(lineEnd.GetPos())

			const float v1_proj = v1.dot(V4asV3_Const(plane));
			const float v2_proj = v2.dot(V4asV3_Const(plane));

			if (fabsf(v2_proj - v1_proj) < 0.0001f)
				throw std::logic_error("the difference of projections is zero");

			const float t = (plane.w() - v1_proj) / (v2_proj - v1_proj);

			//const Vec3 lineStartToEnd = v2 - v1;
			const Vec3 tmp = v1 + (v2 - v1) * t;

			lineStart.SetPos(tmp);

			if(attrList[1])
				lineStart.SetNormal(lineStart.GetNormal() + (lineEnd.GetNormal() - lineStart.GetNormal()) * t);

			if (attrList[ATTR_COLOR_INDEX])
				lineStart.SetColor(lineStart.GetColor(attrList) + (lineEnd.GetColor(attrList) - lineStart.GetColor(attrList)) * t, attrList);
#undef v1
#undef v2
		}

		//note that lineStart will be interpolated to the plane even if its on the right side of it
		int ClipAgainstPlane(const Vec4& plane, int clipped_ind, std::vector<TriangleWrap>& to_append, const bool* attrList)
		{
			auto dist = [&](const Vec3& p) {
				return (plane.x() * p.x() + plane.y() * p.y() + plane.z() * p.z() - plane.w());
			};

			// Create two temporary storage arrays to classify points either side of plane
			// If distance sign is positive, point lies on "inside" of plane

			uint8_t inside_points[3]{};
			uint8_t insidePointCount = 0;

			uint8_t outside_points[3]{};
			uint8_t outsidePointCount = 0;

#ifdef CPUREN_DEBUG
			printf("working with triangle: \n\t1) (%f, %f, %f, %f)\n\t2) (%f, %f, %f, %f)\n\t3) (%f, %f, %f, %f)",
				clip_triag[0].GetPos().x(), clip_triag[0].GetPos().y(), clip_triag[0].GetPos().z(), clip_triag[0].GetPos().w(),
				clip_triag[1].GetPos().x(), clip_triag[1].GetPos().y(), clip_triag[1].GetPos().z(), clip_triag[1].GetPos().w(),
				clip_triag[2].GetPos().x(), clip_triag[2].GetPos().y(), clip_triag[2].GetPos().z(), clip_triag[2].GetPos().w());
#endif
			const Vec3& p1 = V4asV3_Const(to_append[clipped_ind][0].GetPos());
			const Vec3& p2 = V4asV3_Const(to_append[clipped_ind][1].GetPos());
			const Vec3& p3 = V4asV3_Const(to_append[clipped_ind][2].GetPos());
			// Get signed distance of each point in triangle to plane
			const float d0 = dist(p1);
			const float d1 = dist(p2);
			const float d2 = dist(p3);

			if (d0 >= 0) {
				inside_points[insidePointCount++] = 0;
			}
			else {
				outside_points[outsidePointCount++] = 0;
			}

			if (d1 >= 0) {
				inside_points[insidePointCount++] = 1;
			}
			else {
				outside_points[outsidePointCount++] = 1;
			}

			if (d2 >= 0) {
				inside_points[insidePointCount++] = 2;
			}
			else {
				outside_points[outsidePointCount++] = 2;
			}

			// Now classify triangle points, and break the input triangle into 
			// smaller output triangles if required. There are four possible
			// outcomes...

			if (insidePointCount == 0) {
				return 0;
			}

			if (insidePointCount == 3) {
				return 1;
			}

			if (insidePointCount == 1 && outsidePointCount == 2)
			{
				InterpolateToPlane(plane,
					to_append[clipped_ind][outside_points[0]],
					to_append[clipped_ind][inside_points[0]], attrList);
				InterpolateToPlane(plane,
					to_append[clipped_ind][outside_points[1]],
					to_append[clipped_ind][inside_points[0]], attrList);
				return 1;
			}

			if (insidePointCount == 2 && outsidePointCount == 1)
			{
				VertexData* newTrigVerts = new VertexData[3];

				to_append.push_back(TriangleWrap(newTrigVerts));

				TriangleWrap& newTrig = to_append.back();

				const VertexData outsidePointTemp = to_append[clipped_ind][outside_points[0]].Copy(attrList);

				InterpolateToPlane(plane, to_append[clipped_ind][outside_points[0]], to_append[clipped_ind][inside_points[0]], attrList);

				newTrig[0] = to_append[clipped_ind][outside_points[0]].Copy(attrList);
				newTrig[1] = to_append[clipped_ind][inside_points[2]].Copy(attrList);
				newTrig[2] = outsidePointTemp.Copy(attrList);

				InterpolateToPlane(plane, newTrig[2], newTrig[1], attrList);

				return 2;
			}
		}
	}
}

#undef V4asV3
#undef V4asV3_Const