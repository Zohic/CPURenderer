#pragma once
#include <unordered_map>
#include <gmtl/gmtl.h>


namespace cpuRender {

	using Vec3 = gmtl::Vec3f;
	using Mat4x4 = gmtl::Matrix44f;

	class Transform
	{
	private:

	public:
		Vec3 pos, rot, scale;
		Transform() : pos(), rot(), scale(1.0f, 1.0f, 1.0f)
		{

		}
		Transform(Vec3 p) : pos(p), rot(), scale(1.0f, 1.0f, 1.0f)
		{

		}
		Transform(Vec3 p, Vec3 r) : pos(p), rot(r), scale(1.0f, 1.0f, 1.0f)
		{

		}
		Transform(Vec3 p, Vec3 r, Vec3 s) : pos(p), rot(r), scale(s)
		{

		}
	};

	class ByteList {
		const int DataSize[4]{ 4, 8, 12, 4 };
		enum DataType
		{
			FLOAT, FLOAT2, FLOAT3, INT
		};

		std::unique_ptr<char> buffer;
		std::vector<uint32_t> fieldOffset; //reducable
		std::vector<DataType> fieldType;
	};

	class MeshData {
		std::vector<Vec3> vertices;
		std::vector<uint32_t> indices;
		std::vector<std::unique_ptr<char[]>> vertexValues;
	public:
		typedef std::vector<Vec3>::const_iterator vertexIter;

		std::pair<vertexIter, vertexIter> GetVerticesIterator() const {
			return std::make_pair(vertices.begin(), vertices.end());
		}

		void Load() {
			
			

				
		}

	};

	

	class Material {
		uint32_t attrStride, uniformStride, uniqueUniforms;
		ByteList ByteList;
		void (*vertexShader)(Vec3);
	public:
		Material() {

		}

		void FillAttributes(char* dst) {

		}


	public:
		uint32_t GetAttrStride() const { return attrStride; };
	};

	class Camera {
		Transform transform;
		Mat4x4 projectionMatrix;
	public:
		Mat4x4 GetViewMatrix() const {

		}
		Mat4x4 GetProjectionMatrix() const {

		}
	};

	class RenderObject {
		MeshData* mesh;
		Material* material;
		Transform transform;
		
	public:
		void SetUniform() {

		}

		Mat4x4 GetWorldMatrix() const {

		}

		std::pair<MeshData::vertexIter, MeshData::vertexIter> GetVerticesIterator() const {
			return mesh->GetVerticesIterator();
		}
	};

#define ByteBuffer(buff, ind) *(reinterpret_cast<char*>(buff) + ind)
#define FloatBuffer(buff, ind) *(reinterpret_cast<float*>(buff) + ind)
#define IntBuffer(buff, ind) *(reinterpret_cast<int*>(buff) + ind)

	class Renderer {
		std::vector<Material*> materials;
		std::vector<std::vector<RenderObject*>> objectsOfMaterial;
		std::vector<std::unique_ptr<char[]>> vertexBufferOfMaterial;
		std::vector<std::unique_ptr<char[]>> uniformBufferOfMaterial;

		Camera mainCamera;

	public:
		Renderer() {
			
		}
		
		void CreateMat() {
			Material mat;
			
		}

		void UnfoldObjects() {
			for (size_t matInd = 0; matInd < materials.size(); matInd++) {
				for (RenderObject const * const objPtr : objectsOfMaterial[matInd]) {
					const RenderObject& obj = *objPtr;

					char* bufferSegBegin = vertexBufferOfMaterial[matInd].get();
					if (bufferSegBegin)
						delete[] bufferSegBegin;

					
					
					const size_t SEGMENT_SIZE = objectsOfMaterial[matInd].size() * materials[matInd]->GetAttrStride();
					bufferSegBegin = new char[SEGMENT_SIZE];
					memset(bufferSegBegin, 0, SEGMENT_SIZE);

					auto meshIterators = obj.GetVerticesIterator();
					std::vector<Vec3>::const_iterator& vertIter = meshIterators.first;
					const std::vector<Vec3>::const_iterator& endVert = meshIterators.second;

					size_t pointer = 0;
					
					for (; vertIter != endVert; vertIter++) {
						FloatBuffer(bufferSegBegin, pointer++) = (*vertIter)[0];
						FloatBuffer(bufferSegBegin, pointer++) = (*vertIter)[1];
						FloatBuffer(bufferSegBegin, pointer++) = (*vertIter)[2];
						pointer += materials[matInd]->GetAttrStride();
						//for(materials[matInd]->)
					}

					

				}
			}
		}


	};
}

