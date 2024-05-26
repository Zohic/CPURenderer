//#include "Render.h"
//#include "MeshLoader.h"

#include <iostream>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

//#define CPUREN_DEBUG

#include "ResourceStorage.h"
#include "MeshLoader.h"

#include "VertexBuffer.h"
#include "Transformer.h"
#include "CustomizableRasterizer.h"

/*

Data containers:
-MeshData: stores vertrex data (aka attributes);
-Materail: stores global uniforms and links to shaders;
-RenderShape: stores links to mesh and material;
-RenderInstance: stores position, orientation and scale alongside with render shape

Classes and Interfaces:
-IMeshLoader: Loads MeshData from file

-IResourceStorage: must provide convinient way of storing reusable resources like meshes, textures and materials

-IDrawingFunctional: a set of function for showing stuff on the screen

-VertexBufferBase: buffer for vertices

-FrameBufferBase: pixel grid with customizable size and depth (might be stencil or depth buffer)

-TransformerBase: Converts list of RenderInstance to vertex buffer that can be processed by renderer

-RasterizerBase: Renders vertex buffer into framebuffer (depends on VertexBuffer and DrawingFunctional)
*/

/*
rasterizer 
	gets vertices from vertex buffer by groups of 3 and fills triangles using corresponging material

vertex buffer 
	holds vertices and links to materials corresponding to them

transformer
	takes render instances and fills vertex buffer with them while transforming them using vertex shader and clipping triangles

*/


/*
	ResourceStorage storage;
	MeshLoader meshLoader;

	VertexBuffer mainVertexBuffer;
	FragmentBuffer screenBuffer;

	Transformer transformer(&mainVertexBuffer);
	Rasterizer rasterizer(&screenBuffer);

	

	try {
		storage.ReserveMesh("coin");
		storage.ReserveMaterial("simple");
	
		storage.AccessMesh("coin") = meshLoader.Load("coin.gltf);

		storage.AccessMaterial("simple") = new Material(
			ATTR_NORMAL|ATTR_TEXCOORD,
			[](const &VertexShaderInput input) {
				return input.vertexData.position * input.objectData.worldMatrix * input.sceneData.viewMatrix;
			},
			[](const PixelShaderInput &input) {
				return Vec4(input.attributes.normal.x, input.attributes.normal.y, input.attributes.normal.z, 1.0);
			}
		);

		storage.RegisterShape("coin", "coin", "simple");

	} catch(std::exception& exc) {
		printf("error during filling storage: %s", exc.what());
	}

	std::vector<RenderInstance> coins(10);

	for(int i=0; i < 10; i++) {
		coins.push_back(new RenderInstance(Vec3(i*5, 0, 0), storage.GetShape("coin")));
	}

	try {
		transformer.AddObjects(coins.begin(), coins.end());
		transformer.FillVertexBuffer();

		rasterizer.FillFrameBuffer(transformer.FlushBuffer());
	} catch(std::exception& exc) {
		printf("error during rendering: %s", exc.what());
	}
*/

/*
shader example

vertex shader

VertexData for attributes
RenderInstance for transform, material and uniforms
Scene for camera and lights

VertexData TransformAndProject(VertexData& vd, const RenderInstance& ri, const Scene* scene) {

	const Vec4 homCoord = Vec4(vd.GetPos().x(), vd.GetPos().y(), vd.GetPos().z(), 1.0f);
	const Vec4 worldSpace = homCoord * ri.GetWorldMatrix();
	const Vec4 viewSpace = worldSpace * scene->GetCamera()->ViewMatrix();
	const Vec4 clipSpace = viewSpace * scene->GetCamera()->ProjectionMatrix();
	
	const VertexData vdOut;

	vdOut.SetPos(clipSpace);

	return vdOut;
}

Vec4 GrayShade(const VertexData& prevd, const VertexData& postvd, const RenderInstance& ri, const Scene* scene) {
	Vec3 localLightDir = vd.GetPos() - scene->GetLight().GetPos();
}





*/

#define PrintError(text, ...)  \
SetConsoleTextAttribute(hConsole, FOREGROUND_RED);\
printf(text, __VA_ARGS__);\
SetConsoleTextAttribute(hConsole, FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN)\


using namespace cpuRenderSimple;
using namespace cpuRenderBase;


class OLCDrawing final: public IDrawingFunctional {
	olc::PixelGameEngine* eng;
	OLCDrawing() = delete;
public:
	OLCDrawing(olc::PixelGameEngine* ieng) : eng(ieng) {
		if (ieng == nullptr)
			throw std::exception("trying to make OLC drawing with nullptr");
	}
	
	void DrawPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) override {

	}

	void DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint8_t r, uint8_t g, uint8_t b) override {
		eng->DrawLine(x1, y1, x2, y2, olc::Pixel(r, g, b));
	}

	void FillTriangle(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3, uint8_t r, uint8_t g, uint8_t b) override{
		eng->FillTriangle(x1, y1, x2, y2, x3, y3, olc::Pixel(r, g, b));
	}

};

void SkipVert(VertexData& vertex, VertexShaderInput& vInput) {
	DEBUGPRINT("vertex shader: received pos");
	vertex.PrintPos();
	
	vertex.SetPos(vInput.worldMatrix * vertex.GetPos());
	vertex.SetColor(vertex.GetPos(), vInput.availableAttrs);

	DEBUGPRINT("vertex shader: transformed ");
	vertex.PrintPos();

	DEBUGPRINT("vertex shader: color ");
	vertex.PrintColor(vInput.availableAttrs);

	
	vertex.SetPos(vInput.projectionMatrix * vertex.GetPos());

	Vec4 p = vertex.GetPos();
	
	Vec3 n = vertex.GetNormal();


	DEBUGPRINT("vertex shader: normal ");
	vertex.PrintNormal();

	n = vInput.rotationMatrix * n; 
	vertex.SetNormal(n);

	DEBUGPRINT("vertex shader: rotated normal ");
	vertex.PrintNormal();


	DEBUGPRINT("vertex shader: projected ");
	vertex.PrintPos();

	

	//DEBUGPRINT("vertex shader: screen space ");
	//DEBUGPRINT("(%f, %f, %f, %f)\n", p.x(), p.y(), p.z(), p.w());

	//vertex.SetPos(p);
	
}

class MainC: public olc::PixelGameEngine {
public:

	ResourceStorge storage;

	MeshLoader meshLoader;
	

	VertexBuffer vertexBuffer;
	OLCDrawing drawer{ this };
	Transformer<VertexBuffer> transformer = Transformer<VertexBuffer>(&vertexBuffer);
	CustomizableRasterizer<VertexBuffer, OLCDrawing> raster = CustomizableRasterizer<VertexBuffer, OLCDrawing>(&vertexBuffer, &drawer);

	RenderInstance* ri = nullptr;

	float xshift = 0, yshift = 0;
	float cellSize = 10;
	float timer = 0;
	
	MainC() {
		printf("Constructing game\n");
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	Transform f1;

	bool OnUserCreate() override {
		printf("user creating game\n");
		try {
			
			storage.ReserveMesh("triag");
			Material& simpMat = storage.ReserveMaterial("simple", ATTR_POS_MASK | ATTR_NORMAL_MASK | ATTR_COLOR_MASK);
			
			meshLoader.LoadMesh("clockTower.glb", storage.GetMesh("triag"));
			//testCube.glb
			//clockTower.glb
			//rifleV1test.glb
			//scena

			//storage.GetMesh("triag").PrintData();
		
			simpMat.SetVertexShader(SkipVert);
			simpMat.SetUniform<float>("height", 3.14f);
			simpMat.SetUniform<float>("width", 33.14f);
			simpMat.SetUniform<Vec3*>("target", new Vec3(23, 33, 44));

			simpMat.PrintUniforms();

			storage.RegisterRenderShape("test", "triag", "simple");

			transformer.SetCamera(90.0f, 1.0f, 0.01f, 200.0f);
			

			transformer.SetCulling(Transformer<VertexBuffer>::CullType::FRONT);
			
			raster.SetViewPort(0, 0, ScreenWidth(), ScreenHeight());
		}
		catch (std::exception& exc) {
			PrintError("exception during loading stage: \n\t%s\n", exc.what());
			throw exc;
		}

		//mesh.PrintData();

		try {

			ri = new RenderInstance(&(storage.GetShape("test")));
			ri->transform.SetParent(&f1);
			ri->transform.pos = Vec3(0, 90, -395.0f);
			ri->transform.scale = Vec3(20.0f, -20.0f, 20.0f);
		}
		catch (std::exception& exc) {
			PrintError("exception during instancing stage \n\t%s:\n", exc.what());
			throw exc;
		}
		
		return true;
	}
	int cltmp = 0;
	
	bool OnUserUpdate(float dt) override {
		Clear(olc::BLACK);
		xshift += (15.0f * dt + 54.0f*sinf(timer*0.5f)*dt);
		yshift += 10 * dt;
		dt *= 0.05;
		timer += dt;
		cellSize = 10.0f + 0.25f*sinf(timer*13.0f);

		for (float i = 0; i < ScreenWidth(); i++) {
			for (float j = 0; j < ScreenHeight(); j++) {

				int x = (int)(i + xshift) / cellSize;
				int y = (int)(j + yshift) / cellSize;

				if ( (x+y) % 2 == 0) {
					this->Draw(i, j, olc::Pixel(25, 25, 25));
				}else
					this->Draw(i, j, olc::Pixel(55, 55, 55));
			}
		}

		//ri->transform.pos.x() += 1.0f * dt;
		//ri->transform.pos.y() += 1.0f * dt;

		//ri->transform.pos.z() = 130.0f * sinf(timer * 1.2f) - 200.0f;
		//ri->transform.pos.x() = 140.0f * cosf(timer * 3.5f);
		//ri->transform.pos.y() = 30.0f * sinf(timer * 2.0f) * sinf(timer * 5.0f);

		//f1.rot.y() += 3.14f * 1.13f * dt;
		//f1.pos.z() -= 20.13f * dt;

		ri->transform.rot.y() += 3.14f * 3.33f * dt;
		//ri->transform.rot.z() += 3.14f * 1.33f * dt;

		//f1.pos.y() = 3.5f * sinf(timer * 2.0f);

		//ri->transform.scale.x() = 40.0f * sin(timer * 2.0f);

		vertToDraw = ((int)(timer / 0.1f)) % 9;

		if (GetKey(olc::SPACE).bPressed)
			transformer.SetCulling((Transformer<VertexBuffer>::CullType)((++cltmp)%3));

		this->DrawString(20, 20, std::to_string(vertexBuffer.GetVertexCount()));

		try {
			vertexBuffer.ResetIterators(true);
			transformer.ProcessRenderInstance(*ri);
		}
		catch (std::exception& exc) {
			PrintError("Exception during transformation: \n\t%s\n", exc.what());
		}


		try {
			raster.DrawWires();
			DEBUGPRINT("drew wires\n");
		}
		catch (std::exception& exc) {
			PrintError("Exception during rasterization: \n\t%s\n", exc.what());
		}
		
		return true;
	}

};



int main() {
	

	try {
		MainC game;
		printf("constructed game\n");
		if (game.Construct(700, 700, 1, 1))
			game.Start();


		printf("game begun\n\t");
	}
	catch (std::exception& exc) {
		printf("exception during preparation stage:\n\t");
		printf("%s\n", exc.what());
	}

	std::cout << sizeof(std::vector<float>) << std::endl;
	std::cout << sizeof(std::vector<uint32_t>) << std::endl;

	

	return 0;
}