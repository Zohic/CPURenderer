#pragma once

#include "Camera.h"
#include "RenderInstance.h"
#include "TriangleWrap.h"
#include <Windows.h>

static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

#define V4asV3(v4) (*(reinterpret_cast<Vec3*>(&v4)))
#define V4asV3_Const(cv4) (*(reinterpret_cast<const Vec3*>(&cv4)))

namespace cpuRenderBase {

	struct Quaternion
	{
		float x, y, z, w;

		void FromEuler(Vec3 euler)// for ZYX sequence
		{
			//row, pitch, yaw
			double cr = cos(euler.x() * 0.5);
			double sr = sin(euler.x() * 0.5);
			double cp = cos(euler.y() * 0.5);
			double sp = sin(euler.y() * 0.5);
			double cy = cos(euler.z() * 0.5);
			double sy = sin(euler.z() * 0.5);

			w = cr * cp * cy + sr * sp * sy;
			x = sr * cp * cy - cr * sp * sy;
			y = cr * sp * cy + sr * cp * sy;
			z = cr * cp * sy - sr * sp * cy;
		}

		Vec3 toEuler()
		{
			Vec3 angles;

			// roll (x-axis rotation)
			float sinr_cosp = 2 * (w * x + y * z);
			float cosr_cosp = 1 - 2 * (x * x + y * y);
			angles.x() = std::atan2(sinr_cosp, cosr_cosp);

			// pitch (y-axis rotation)
			float sinp = std::sqrt(1 + 2 * (w * y - x * z));
			float cosp = std::sqrt(1 - 2 * (w * y - x * z));
			angles.y() = 2 * std::atan2(sinp, cosp) - PI_2;

			// yaw (z-axis rotation)
			float siny_cosp = 2 * (w * z + x * y);
			float cosy_cosp = 1 - 2 * (y * y + z * z);
			angles.z() = std::atan2(siny_cosp, cosy_cosp);

			return angles;
		}

		void FromAxisAngle(Vec3 axis, float radians) {
			float sinAngle;

			radians *= 0.5f;
			axis.normalize();
			sinAngle = sin(radians);
			x = axis.x() * sinAngle;
			y = axis.y() * sinAngle;
			z = axis.z() * sinAngle;
			w = cos(radians);
		}

		void Multiply(Quaternion rhs) {

			x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
			y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
			z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;
			w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
		}

		void Rotate(Vec3 axis, float radians) {
			Quaternion rotationQuaternion;
			rotationQuaternion.FromAxisAngle(axis, radians);
			Multiply(rotationQuaternion);
		}
	};

	class LightSource {
		Vec3 position;
		Vec3 color;
	};

	class Environment {
		std::vector<Camera> cameras;
		std::vector<LightSource> lights;
	};

	class Scene {
		std::unordered_map<const Material*, std::vector<RenderInstance>> instancesByMaterial;
		Camera* camera;

		RenderInstance& InsertRenderInstance(const RenderInstance& rendInst) {
			const Material* matPtr = &(rendInst.GetShape()->GetMaterial());
			instancesByMaterial[matPtr].push_back(rendInst);
			return *(instancesByMaterial.at(matPtr).end() - 1);
		}

		std::unordered_map<const Material*, std::vector<RenderInstance>>::const_iterator GetInsatneces() const {
			return instancesByMaterial.cbegin();
		}


	};

	class VertexBufferBase {
	public:
		//did you know that functions defined in classes are implicitly inline?
		virtual void InsertVertex(VertexData&& vert, const Material* const mat) = 0;
		virtual const VertexData& GetVert() = 0;
		virtual const VertexData* GetVertPtr() = 0;
		virtual size_t GetVertexCount() const = 0;
		virtual const Material& GetMaterial() const = 0;
		virtual bool End() const = 0;
		virtual void Clear() = 0;
		virtual void ResetIterators(bool recount) = 0;
	};

	template<typename VertexBufferType = VertexBufferBase>
	class TransformerBase {
	private:

		TransformerBase() = delete;
		//Vec3 viewerPosition;
		//Vec3 viewDirection;


	protected:
		VertexBufferType* buffer = nullptr;
		Camera cam;
	public:

		const Camera& GetCamRef() const {
			return cam;
		}
		TransformerBase(VertexBufferType* buff) : buffer(buff) {
			if (!buff)
				throw std::logic_error("vertex buffer pointer is null");

			cam.transform.pos = { 0, 0, 0 };
		}
		virtual void ProcessRenderInstance(const RenderInstance& renderObject) = 0;
	};

	class IDrawingFunctional {
	public:
		virtual void DrawPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) const = 0;
		virtual void DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint8_t r, uint8_t g, uint8_t b) const = 0;
		virtual void FillTriangle(
			uint32_t x1, uint32_t y1, 
			uint32_t x2, uint32_t y2, 
			uint32_t x3, uint32_t y3, 
			uint8_t r, uint8_t g, uint8_t b) const = 0;
		virtual void PrintText(uint32_t x, uint32_t y, const std::string& str) const = 0;

	};

	class RasterizerBase {
	private:
		RasterizerBase() = delete;
	protected:
		VertexBufferBase* vertexBuffer;
		IDrawingFunctional* drawingTool;

		Vec2 viewportPos;
		Vec2 viewportSize;

	public:

		const Vec2& GetViewportPos() const {
			return viewportPos;
		}

		const Vec2& GetViewportSize() const {
			return viewportSize;
		}

		RasterizerBase(VertexBufferBase* ivertBuffer, IDrawingFunctional* idrawingTool) : vertexBuffer(ivertBuffer), drawingTool(idrawingTool) {
			if (!ivertBuffer)
				throw std::logic_error("vertex buffer pointer is null");
			if (!idrawingTool)
				throw std::logic_error("drawing tool pointer is null");
		}

		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
			viewportPos  << x, y;
			viewportSize << w, h;
		}
		virtual void DrawWires() = 0;
		virtual void RenderTriangles() = 0;
	};

	namespace utils {

		static void PrintMatrix(Mat4x4& mat) {
			DEBUGPRINT("Mat4x4:\n");
			DEBUGPRINT("%f8, %f8, %f8, %f8\n", mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
			DEBUGPRINT("%f8, %f8, %f8, %f8\n", mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
			DEBUGPRINT("%f8, %f8, %f8, %f8\n", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
			DEBUGPRINT("%f8, %f8, %f8, %f8\n", mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
		}

		//lineStart will be exchanged with interpolated value\
		//note that lineStart will be interpolated to the plane even if its on the right side of it
		static void InterpolateToPlane(const Vec4& plane, VertexData& lineStart, const VertexData& lineEnd, const bool* attrList) {

#define v1 V4asV3_Const(lineStart.GetPos())
#define v2 V4asV3_Const(lineEnd.GetPos())

			const float v1_proj = v1.dot(V4asV3_Const(plane));
			const float v2_proj = v2.dot(V4asV3_Const(plane));

			if (fabsf(v2_proj - v1_proj) < 0.0001f)
				throw std::logic_error("difference of projections is zero");

			const float t = (plane.w() - v1_proj) / (v2_proj - v1_proj);

			//const Vec3 lineStartToEnd = v2 - v1;
			const Vec3 tmp = v1 + (v2 - v1) * t;
			lineStart.SetPos(tmp);

			lineStart.SetNormal(lineStart.GetNormal() + (lineEnd.GetNormal() - lineStart.GetNormal()) * t);
			if (attrList[ATTR_COLOR_INDEX])
				lineStart.SetColor(lineStart.GetColor(attrList) + (lineEnd.GetColor(attrList) - lineStart.GetColor(attrList)) * t, attrList);
#undef v1
#undef v2
		}
		static int ClipAgainstPlane(const Vec4& plane, int clipped_ind, std::vector<TriangleWrap>& to_append, const bool* attrList)
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



