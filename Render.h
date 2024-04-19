#pragma once
#include <unordered_map>
#include "gmtl/gmtl.h"
#include "cgltf/cgltf.h"
#include <string>

namespace cpuRenderBase {
	
	using Vec2 = gmtl::Vec2f;
	using Vec3 = gmtl::Vec3f;
	using Vec4 = gmtl::Vec4f;
	using Mat4x4 = gmtl::Matrix44f;

	class MeshData {
		std::vector<float> vertices;
		std::vector<float> normal;
		std::vector<float> tangent;
		std::vector<float> texcoord;
		std::vector<float> color;

		std::vector<uint32_t> indices;

		uint8_t attributeAvailabiltyMask;
		uint32_t triangleCount;

	public:
		static constexpr size_t VERTEX_OFFSET = 0;
		static constexpr size_t NORMAL_OFFSET = 1;
		static constexpr size_t TANGENT_OFFSET = 2;
		static constexpr size_t TEXCOORD_OFFSET = 3;
		static constexpr size_t COLOR_OFFSET = 4;

		static constexpr uint8_t ATTR_POS_MASK = 1;
		static constexpr uint8_t ATTR_NORMAL_MASK = 2;
		static constexpr uint8_t ATTR_TANGENT_MASK = 4;
		static constexpr uint8_t ATTR_TEXCOORD_MASK = 8;
		static constexpr uint8_t ATTR_COLOR_MASK = 16;


		MeshData() : vertices(), normal(), tangent(), texcoord(), color(), indices(), attributeAvailabiltyMask(0), triangleCount(0) {

		}


		MeshData(MeshData&& old) noexcept :
			vertices(std::move(old.vertices)),
			normal(std::move(old.normal)),
			tangent(std::move(old.tangent)),
			texcoord(std::move(old.texcoord)),
			color(std::move(old.color)),
			indices(std::move(old.indices)) {

			attributeAvailabiltyMask = old.attributeAvailabiltyMask;
			triangleCount = old.triangleCount;

			old.attributeAvailabiltyMask = 0;
			old.triangleCount = 0;
		}
		MeshData& operator=(MeshData&& old) noexcept {
			vertices = std::move(old.vertices);
			normal = std::move(old.normal);
			tangent = std::move(old.tangent);
			texcoord = std::move(old.texcoord);
			color = std::move(old.color);
			indices = std::move(old.indices);

			attributeAvailabiltyMask = old.attributeAvailabiltyMask;
			old.attributeAvailabiltyMask = 0;
		}
		void SetList(size_t ind, std::vector<float>&& list) noexcept {
			if (ind > COLOR_OFFSET) {
				throw std::exception("invalid list ind");
			}

			*(((std::vector<float>*)(this)) + ind) = list;
		}
		void SetIndices(std::vector<uint32_t>&& list) {
			indices = list;
		}
		void PrintData() const noexcept {
			for (int i = 0; i < vertices.size(); i += 3) {
				printf("vertex %i: (%f, %f, %f)\n", i / 3 + 1, vertices[i], vertices[i + 1], vertices[i + 2]);
			}

			for (int i = 0; i < normal.size(); i += 3) {
				printf("normal %i: (%f, %f, %f)\n", i / 3 + 1, normal[i], normal[i + 1], normal[i + 2]);
			}

			for (int i = 0; i < tangent.size(); i += 4) {
				printf("tangent %i: (%f, %f, %f, %f)\n", i / 4 + 1, tangent[i], tangent[i + 1], tangent[i + 2], tangent[i + 3]);
			}

			for (int i = 0; i < texcoord.size(); i += 2) {
				printf("texcoord %i: (%f, %f)\n", i / 2 + 1, texcoord[i], texcoord[i + 1]);
			}

			for (int i = 0; i < color.size(); i += 4) {
				printf("color %i: (%f, %f, %f, %f)\n", i / 4 + 1, color[i], color[i + 1], color[i + 2], color[i + 3]);
			}

			for (int i = 0; i < indices.size(); i += 3) {
				printf("index %i: (%i, %i, %i)\n", i / 3 + 1, indices[i], indices[i + 1], indices[i + 2]);
			}
		}

		void EmplaceAttributesOfTriangle(size_t tri, unsigned int attr, float* dst) const {
			const size_t triag = tri * 3;
			const std::vector<float>* const list = (((std::vector<float>*)(this)) + attr);

			*(dst + 0) = (*list)[indices[triag]];
			*(dst + 1) = (*list)[indices[triag] + 1];
			*(dst + 2) = (*list)[indices[triag] + 2];

			*(dst + 3) = (*list)[indices[triag + 1] ];
			*(dst + 4) = (*list)[indices[triag + 1] + 1];
			*(dst + 5) = (*list)[indices[triag + 1] + 2];

			*(dst + 6) = (*list)[indices[triag + 2]];
			*(dst + 7) = (*list)[indices[triag + 2] + 1];
			*(dst + 8) = (*list)[indices[triag + 2] + 2];

		}

		void SetTriangleCount(uint32_t cnt) {
			triangleCount = cnt;
		}

		float GetAttr(size_t attrInd, size_t ind) const {
			if (attrInd > COLOR_OFFSET) 
				throw std::exception("invalid list ind");

			return (*(((std::vector<float>*)(this)) + ind))[ind];
		}

		size_t GetVertexCount() const {
			return vertices.size();
		}
		uint32_t GetTriangleCount() const {
			return triangleCount;
		}
		uint8_t GetAttrMask() const {
			return attributeAvailabiltyMask;
		}

	};
	
	class VertexShaderInput {
		/*const Vec2* const uv;
		const Vec3* const pos;
		const Vec3* const norm;
		const Vec4* const tangent;
		const Vec4* const color;*/

	public:
		VertexShaderInput() {

		}
	};
	typedef Vec3(*VertexShader)(const VertexShaderInput& v_in);

	class Material {

		VertexShader vertexShader = nullptr;
		void (*pixelShader)(Vec3, float*) = nullptr;

		uint8_t attributeRequirementMask = 0;
	public:
		Material() {

		}

		const void* const VertexShader() const {
			return vertexShader;
		}

		uint8_t GetAttrMask() const {
			return attributeRequirementMask;
		}
		void FillAttributes(char* dst) {

		}


	public:

	};
	
	class RenderShape {
	private:
		MeshData* mesh = nullptr;
		Material* material = nullptr;
		RenderShape() {

		}
	public:
		RenderShape(MeshData* imesh, Material* imat): mesh(imesh), material(imat) {

		}
		void SetMesh(const MeshData* imesh) {
			mesh = const_cast<MeshData*>(imesh);
		}
		void SetMaterial(const Material* mat) {
			const uint8_t& req = mat->GetAttrMask();
			const uint8_t& avl = mesh->GetAttrMask();

			bool hasAttr = (req & avl) == req;

			if (hasAttr) {
				material = const_cast<Material*>(mat);
			}
			else
				throw std::exception("the material reqires more attributes that the mesh has");
		}

		void SetUniform() {

		}

		const MeshData& GetMesh() const {
			return *mesh;
		}
		const Material& GetMaterial() const {
			return *material;
		}
	
		

	};

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
	class RenderInstance {
	public:
		Transform transform; //public since any value is valid
	private:
		RenderShape *shape;
		RenderInstance() {

		}
	public:
		RenderInstance(RenderShape* ishapeID) : transform(), shape(ishapeID) {

		}
		RenderInstance(Transform itrans, RenderShape* ishapeID) : transform(itrans), shape(ishapeID) {

		}
		const RenderShape* GetShape() const {
			return shape;
		}
	};

	class VertexBuffer {
		std::vector<float> data;
		std::vector<size_t> materialOffset;

	public:
		VertexBuffer() : data(200) {

		}

		void InsertVertex(const Vec3& v) {
			data.push_back(v.x());
			data.push_back(v.y());
			data.push_back(v.z());
		}

		void Clear() {
			data.clear();
			materialOffset.clear();
		}
	};
	
	class FragmentBuffer {
		uint32_t width, height;
		float* data;
		FragmentBuffer() { }
	public:

		FragmentBuffer(uint32_t iwidth, uint32_t iheight): width(iwidth), height(iheight), data(new float[width*height*3]) {
			if (iwidth == 0 || iheight == 0) {
				if (data)
					delete[] data;
				throw std::exception("attempting to create fragment buffer with zero width or/and height");
			}
		}

		~FragmentBuffer() {
			if (data)
				delete[] data;
		}

	};

	class IResourceStorge {
	public:
		virtual void ReserveMesh(std::string& name) = 0;
		virtual void ReserveMaterial(std::string& name) = 0;

		virtual MeshData& GetMesh(std::string& name) = 0;
		virtual Material& GetMaterial(std::string& name) = 0;

		virtual void RegisterRenderShape(std::string& shapeName, std::string& meshName, std::string& matName) = 0;
		virtual const RenderShape& GetShape(std::string& shapeName) const = 0;

	};

	class IMeshLoader {
		virtual void LoadMesh(const char* fileName, MeshData& outMesh) const noexcept = 0;
	};


	class Camera {
		Mat4x4 projectionMatrix;
	public:
		Transform transform;

		Mat4x4 GetViewMatrix() const {

		}
		Mat4x4 GetProjectionMatrix() const {

		}
	};

	class Environment {
		Camera* camera;
	
	public:
		

	};

	class TransformerBase {

	protected:
		IResourceStorge* storage = nullptr;
		VertexBuffer* buffer = nullptr;

		TransformerBase() {}
	public:
		Camera cam;
		
		TransformerBase(IResourceStorge* stor, VertexBuffer* buff) : storage(stor), buffer(buff) {

		}
		virtual void ProcessRenderObject(const RenderInstance& renderObject) = 0;
	};
	
	class RasterizerBase {
	protected:
		VertexBuffer* vertBuffer;
		FragmentBuffer* fragBuffer;
		RasterizerBase() {}
	public:
		RasterizerBase(VertexBuffer* ivertBuffer, FragmentBuffer* ifragBuffer) : vertBuffer(ivertBuffer), fragBuffer(ifragBuffer) {

		}

		DrawWireTrinagles() {

		}
	};
	
}

