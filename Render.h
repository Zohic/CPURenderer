#pragma once
#include <unordered_map>
#include "gmtl/gmtl.h"
#include <string>
#include <variant>

#ifdef CPUREN_DEBUG
#define DEBUGPRINT(text, ...) printf(text, __VA_ARGS__);
#endif // CPUREN_DEBUG

#ifndef CPUREN_DEBUG
#define DEBUGPRINT(text, ...) ;
#endif // !CPUREN_DEBUG

HANDLE  hConsole;


namespace cpuRenderBase {
	using Vec2 = gmtl::Vec2f;
	using Vec3 = gmtl::Vec3f;
	using Vec4 = gmtl::Vec4f;
	using Mat4x4 = gmtl::Matrix44f;

	class MeshData {
		std::vector<Vec3> vertices;
		std::vector<Vec3> normal;
		std::vector<Vec4> tangent;
		std::vector<Vec2> texcoord;
		std::vector<Vec4> color;

		std::vector<uint32_t> indices;

		uint8_t attributeAvailabiltyMask;
		uint32_t triangleCount;

	public:
		static constexpr size_t ATTR_POS = 0Ui64;
		static constexpr size_t ATTR_NORMAL = 1Ui64;
		static constexpr size_t ATTR_TANGENT = 2Ui64;
		static constexpr size_t ATTR_TEXCOORD = 3Ui64;
		static constexpr size_t ATTR_COLOR = 4Ui64;

		static constexpr uint8_t ATTR_POS_MASK = 1Ui8;
		static constexpr uint8_t ATTR_NORMAL_MASK = 2Ui8;
		static constexpr uint8_t ATTR_TANGENT_MASK = 4Ui8;
		static constexpr uint8_t ATTR_TEXCOORD_MASK = 8Ui8;
		static constexpr uint8_t ATTR_COLOR_MASK = 16Ui8;


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
		
		//this works because Vec2, Vec3, Vec4 classes have only one field as array of floats
		void SetAttr(size_t ind, std::vector<float>&& list) {
			if (ind > ATTR_COLOR) {
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

		void SetAttributeAvailability(uint8_t mask) {
			attributeAvailabiltyMask = mask;
		}

		/// <summary>
		///
		/// </summary>
		/// <param name="ind"> - index of vertex in the list of indices</param>
		/// <returns>Vec3 hehe</returns>
		const Vec3& GetPos(size_t ind) const {
			if (ind > indices.size())
				throw std::exception("Getting pos attribute out of bounds");
			return vertices[indices[ind]];
		}

		const Vec3& GetNormal(size_t ind) const {
			if (ind > indices.size())
				throw std::exception("Getting normal attribute out of bounds");
			return normal[indices[ind]];
		}

		const Vec4& GetTangent(size_t ind) const {
			if (ind > indices.size())
				throw std::exception("Getting tangent attribute out of bounds");
			return tangent[indices[ind]];
		}


		size_t GetVertexCount() const {
			return vertices.size() / 3;
		}
		uint32_t GetTriangleCount() const {
			return triangleCount;
		}
		uint8_t GetAttrMask() const {
			return attributeAvailabiltyMask;
		}

	};
	class VertexData {
		std::unique_ptr<float[]> attributes;
		inline static size_t GetAttrFloatSize(bool attrMask[5]) {
			return 4ui64 +
				3ui64 * static_cast<unsigned int>(attrMask[MeshData::ATTR_NORMAL]) +
				4ui64 * static_cast<unsigned int>(attrMask[MeshData::ATTR_TANGENT]) +
				2ui64 * static_cast<unsigned int>(attrMask[MeshData::ATTR_TEXCOORD]) +
				4ui64 * static_cast<unsigned int>(attrMask[MeshData::ATTR_COLOR]);

		}

		VertexData(const VertexData& other) = delete;
		VertexData& operator=(const VertexData& other) = delete;

	public:
		VertexData() {

		}
		VertexData(VertexData&& old) noexcept {
			attributes = std::move(old.attributes);
		}
		VertexData& operator=(VertexData&& old) noexcept {
			attributes = std::move(old.attributes);
			return *this;
		}

		void SetSize(bool attrList[5]) {
			attributes.reset(new float[GetAttrFloatSize(attrList)]);
		}
		void SetFromMesh(const MeshData* const mesh, bool attrReqList[5], size_t vertInd) {
			attributes.reset(new float[GetAttrFloatSize(attrReqList)]);

			//DEBUGPRINT("have set size from mesh: %i\n", GetAttrFloatSize(attrReqList));

			SetPos(mesh->GetPos(vertInd));
			//DEBUGPRINT("have set pos from mesh: ");
			Print();
			if (attrReqList[1])
				SetNormal(mesh->GetNormal(vertInd));

		}

		void SetPos(const Vec3& pos) {
			attributes.get()[0] = pos.x();
			attributes.get()[1] = pos.y();
			attributes.get()[2] = pos.z();
			attributes.get()[3] = 1.0f;
		}
		void SetPos(const Vec4& pos) {
			attributes.get()[0] = pos.x();
			attributes.get()[1] = pos.y();
			attributes.get()[2] = pos.z();
			attributes.get()[3] = pos.w();
		}
		void SetNormal(const Vec3& norm) {
			attributes.get()[4] = norm.x();
			attributes.get()[5] = norm.y();
			attributes.get()[6] = norm.z();
		}
		void SetTangent(const Vec4& tang, bool attrMask[5]) {
			const int offset = 4 + 3 * static_cast<unsigned int>(attrMask[1]);
			attributes.get()[offset + 0] = tang.x();
			attributes.get()[offset + 1] = tang.y();
			attributes.get()[offset + 2] = tang.z();
			attributes.get()[offset + 3] = tang.w();
		}

		const Vec4& GetPos() const {
			if (!attributes.get())
				throw std::exception("cannot GetPos(): vertex data is null\n");
			return Vec4(attributes.get()[0], attributes.get()[1], attributes.get()[2], attributes.get()[3]);
		}
		Vec3 GetNormal() const {

			const float* const nums = (attributes.get() + 4);
			return Vec3(nums[0], nums[1], nums[2]);
		}
		Vec4 GetTangent(bool attrMask[5]) const {
			const float* const nums = (attributes.get()
				+ 4u
				+ 3u * static_cast<unsigned int>(attrMask[1]));

			return Vec4(nums[0], nums[1], nums[2], nums[3]);
		}

		void Print() const {
			printf("\tpos is: (%f, %f, %f, %f)\n", GetPos().x(), GetPos().y(), GetPos().z(), GetPos().w());
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

		Mat4x4 GetTranslationMatrix() const {
			Mat4x4 mTrans;
			mTrans[0][3] = pos.x();
			mTrans[1][3] = pos.y();
			mTrans[2][3] = pos.z();
			mTrans[3][3] = 1.0f;

			return mTrans;
		}

		Mat4x4 GetScaleMatrix() const {
			Mat4x4 mScale;

			mScale[0][0] = scale.x();
			mScale[1][1] = scale.y();
			mScale[2][2] = scale.z();
			mScale[3][3] = 1.0f;

			return mScale;
		}

		Mat4x4 GetRotationMatrix() const {
			Mat4x4 mRot;

			mRot[0][0] = cosf(rot.z());
			mRot[0][1] = sinf(rot.z());
			mRot[1][0] = -sinf(rot.z());
			mRot[1][1] = cosf(rot.z());

			mRot[3][3] = 1.0f;

			return mRot;
		}
	};

	void PrintMatrix(Mat4x4& mat) {
		printf("Mat4x4:\n");
		printf("%f8, %f8, %f8, %f8\n", mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
		printf("%f8, %f8, %f8, %f8\n", mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
		printf("%f8, %f8, %f8, %f8\n", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
		printf("%f8, %f8, %f8, %f8\n", mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
	}

	class Camera {
		Mat4x4 projectionMatrix;
		enum class CameraType{
			ORTHOGRAPHIC,
			PERSPECTIVE
		};
	public:
		Transform transform;

		Mat4x4 GetViewMatrix() const {

		}
		Mat4x4 GetProjectionMatrix() const {
			return projectionMatrix;
		}

		void SetFrustum(float fovY, float aspectRatio, float front, float back)
		{
			DEBUGPRINT("SETTING FRUSTUM\n");
			const float DEG2RAD = acos(-1.0f) / 180;

			float tangent = tan(fovY / 2 * DEG2RAD);    // tangent of half fovY
			float top = front * tangent;              // half height of near plane
			float right = top * aspectRatio;          // half width of near plane

			// params: left, right, bottom, top, near(front), far(back)
			
			projectionMatrix[0][0] = front / right;
			projectionMatrix[1][1] = front / top;
			projectionMatrix[2][2] = -(back + front) / (back - front);
			projectionMatrix[3][2] = -1;
			projectionMatrix[2][3] = -(2 * back * front) / (back - front);
			projectionMatrix[3][3] = 0;
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

	typedef std::variant<float, Vec2*, Vec3*, Vec4*, Mat4x4*> UniformValue;
	class UnifromStorage {
	protected:
		std::unordered_map<std::string, UniformValue> uniforms;
	public:
		template<typename T>
		const T GetUniform(std::string& name) const {
			if (!std::holds_alternative<T>(uniforms[name]))
				throw std::exception("no uniform of such type or name (GET)");

			return std::get<T>(uniforms[name]);
		}

		template<typename T>
		const T GetUniform(std::string&& name) const {
			if (!std::holds_alternative<T>(uniforms[name]))
				throw std::exception("no uniform of such type or name (GET)");

			return std::get<T>(uniforms[name]);
		}

		template<typename T>
		void SetUniform(std::string& name, T v) {
			uniforms[name] = v;
		}

		template<typename T>
		void SetUniform(std::string&& name, T v) {
			uniforms[name] = v;
		}

		void PrintUniforms() {
			printf("list of unifrom values:\n");
			for (auto uni = uniforms.begin(); uni != uniforms.end(); uni++) {
				const std::string& name = (*uni).first;

				UniformValue value = (*uni).second;
				
				if (std::holds_alternative<float>(value)) {
					printf("\t%s = %f\n", name, std::get<float>(value));
				}
				else if (std::holds_alternative<Vec2*>(value)) {
					printf("\t%s = (%f, %f)\n", name, std::get<Vec2*>(value)->x(), std::get<Vec2*>(value)->y());
				}
				else if (std::holds_alternative<Vec3*>(value)) {
					printf("\t%s = (%f, %f, %f)\n", name, std::get<Vec3*>(value)->x(), std::get<Vec3*>(value)->y(), std::get<Vec3*>(value)->z());
				}
				else if (std::holds_alternative<Vec4*>(value)) {
					printf("\t%s = (%f, %f, %f, %f)\n", name, 
						std::get<Vec4*>(value)->x(), std::get<Vec4*>(value)->y(), std::get<Vec4*>(value)->z(), std::get<Vec4*>(value)->w());
				}
				else if (std::holds_alternative<Mat4x4*>(value)) {
					printf("\t%s = matrix4x4\n", name);
				}

				
			}
		}

	};
	class VertexShaderInput {
	public:
		Vec3 cameraPosition;
		Vec3 cameraDirection;
		Mat4x4 projectionMatrix;
		Mat4x4 viewMatrix;

		Mat4x4 translationMatrix;
		Mat4x4 scalingMatrix;
		Mat4x4 rotationMatrix;

		UnifromStorage* uniforms;
		std::vector<LightSource> lights;

		bool availableAttrs[5];

	};

	typedef void (*VertexShader)(VertexData& v_in, VertexShaderInput& inp);
	typedef Vec4 (*FragmentShader)(const VertexData& f_in);

	const uint8_t atrr_offsets[5] = {
		MeshData::ATTR_POS_MASK,
		MeshData::ATTR_NORMAL_MASK,
		MeshData::ATTR_TANGENT_MASK,
		MeshData::ATTR_TEXCOORD_MASK,
		MeshData::ATTR_COLOR_MASK };
	class Material: public UnifromStorage {

		VertexShader vertexShader = nullptr;
		FragmentShader fragmentShader = nullptr;

		uint8_t attributeRequirementMask;
		bool attributeRequirementList[5];

		Material() = delete;
		Material(const Material& other) = delete;
		Material(Material&& old) = delete;

		Material& operator=(const Material& other) = delete;
		Material& operator=(Material&& old) = delete;

	public:

#define AttrFromMask(attr) (attributeRequirementMask & atrr_offsets[attr]) >> attr
		Material(uint8_t mask): attributeRequirementMask(mask){

			if (!(AttrFromMask(MeshData::ATTR_POS)))
				throw std::exception("material has no POSITION requirement");

			attributeRequirementList[0] = true;
			attributeRequirementList[1] = AttrFromMask(MeshData::ATTR_NORMAL);
			attributeRequirementList[2] = AttrFromMask(MeshData::ATTR_TANGENT);
			attributeRequirementList[3] = AttrFromMask(MeshData::ATTR_TEXCOORD);
			attributeRequirementList[4] = AttrFromMask(MeshData::ATTR_COLOR);
		}
#undef AttrFromMask(attr)

		Material(bool list[5]): attributeRequirementList() {

			if (!list[0])
				throw std::exception("material has no POSITION requirement");

			attributeRequirementMask =
				1ui8 |
				atrr_offsets[1] * list[1] |
				atrr_offsets[2] * list[2] |
				atrr_offsets[3] * list[3] |
				atrr_offsets[4] * list[4];

			memcpy(attributeRequirementList, list, 5ui64 * sizeof(bool));
		}

		void SetVertexShader(const VertexShader vs) {
			if (vs == nullptr)
				throw std::exception("setting vertex shader to null");
			vertexShader = vs;
		}
		void SetFragmentShader(const FragmentShader fs) {
			if (fs == nullptr)
				throw std::exception("setting fragment shader to null");
			fragmentShader = fs;
		}

		const VertexShader GetVertexShader() const {
			if (!vertexShader)
				throw std::exception("trying to access vertex shader that is not existent");
			return vertexShader;
		}
		const FragmentShader GetFragmentShader() const {
			if (!vertexShader)
				throw std::exception("trying to access vertex shader that is not existent");
			return fragmentShader;
		}

		uint8_t GetAttrMask() const {
			return attributeRequirementMask;
		}

		const UnifromStorage* GetUniforms() const {
			return this;
		}

		const bool* GetAttrList() const {
			return attributeRequirementList;
		}
		uint8_t HasAttr(size_t attr) const {
			return attributeRequirementList[attr];
		}
		size_t AttributesSize() const {
			return 3ui64 +
				HasAttr(MeshData::ATTR_NORMAL) * 3ui64 +
				HasAttr(MeshData::ATTR_TANGENT) * 4ui64 +
				HasAttr(MeshData::ATTR_TEXCOORD) * 2ui64 +
				HasAttr(MeshData::ATTR_COLOR) * 4ui64;
		}
	};
	
	class RenderShape {
	private:
		MeshData* mesh;
		Material* material;
		RenderShape() = delete;
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
			if (!mesh)
				throw std::exception("trying to access mesh that is not existent");
			return *mesh;
		}
		const Material& GetMaterial() const {
			return *material;
		}
	
		

	};

	
	class RenderInstance {
	public:
		Transform transform; //public since any value is valid
	private:
		RenderShape *shape;

		RenderInstance() = delete;
		//makes no sence to move objects of this class
	public:
		RenderInstance(const RenderShape* ishapeID) : transform(), shape(const_cast<RenderShape*>(ishapeID)) {

		}
		RenderInstance(Transform itrans, const RenderShape* ishapeID) : transform(itrans), shape(const_cast<RenderShape*>(ishapeID)) {

		}

		RenderInstance(const RenderInstance& other): transform(other.transform), shape(other.shape) {

		}

		RenderInstance& operator=(const RenderInstance& other) {
			transform = other.transform;
			shape = other.shape;
		}

		const RenderShape* GetShape() const {
			return shape;
		}
	};

	
	/*class FragmentBuffer {
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

	};*/

	class IMeshLoader {
		virtual void LoadMesh(const char* fileName, MeshData& outMesh) const = 0;
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
		virtual void InsertVertex(VertexData& vert, const Material* const mat) = 0;
		virtual const VertexData& GetVert() = 0;
		virtual size_t GetVertexCount() const = 0;
		virtual const Material& GetMaterial() const = 0;
		virtual bool End() const = 0;
		virtual void Clear() = 0;
		virtual void ResetIterators() = 0;
	};
	
	template<typename VertexBufferType>
	class TransformerBase {
	private:
		
		TransformerBase() = delete;
		//Vec3 viewerPosition;
		//Vec3 viewDirection;
		

	protected:
		VertexBufferType* buffer = nullptr;
		Camera cam;
	public:
		
		
		TransformerBase(VertexBufferType* buff) : buffer(buff) {
			if (!buff)
				throw std::exception("vertex buffer pointer is null");
		}
		virtual void ProcessRenderInstance(const RenderInstance& renderObject) = 0;
	};

	class IDrawingFunctional {
		virtual void DrawPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) = 0;
		virtual void DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint8_t r, uint8_t g, uint8_t b) = 0;
		virtual void FillTriangle(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3, uint8_t r, uint8_t g, uint8_t b) = 0;
	};

	template<typename VertexBufferType = VertexBufferBase, typename DrawingFunctional = IDrawingFunctional>
	class RasterizerBase {
	private:
		RasterizerBase() {}
	protected:
		VertexBufferType* vertexBuffer;
		DrawingFunctional* drawingTool;
	public:
		
		RasterizerBase(VertexBufferType* ivertBuffer, DrawingFunctional* idrawingTool) : vertexBuffer(ivertBuffer), drawingTool(idrawingTool) {
			if (!ivertBuffer)
				throw std::exception("vertex buffer pointer is null");
			if (!idrawingTool)
				throw std::exception("drawing tool pointer is null");
		}

		virtual void DrawWires() = 0;
		virtual void RenderTriangles() = 0;
	};
}

