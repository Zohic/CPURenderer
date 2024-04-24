//#include "Render.h"
//#include "MeshLoader.h"

#include <iostream>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

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

	void DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2) override {
		eng->DrawLine(x1, y1, x2, y2, olc::WHITE);
	}
};

class MainC: public olc::PixelGameEngine {
public:

	ResourceStorge storage;

	MeshLoader meshLoader;
	MeshData mesh;

	VertexBuffer vertexBuffer;
	OLCDrawing drawer{ this };
	Transformer<VertexBuffer> transformer = Transformer<VertexBuffer>(&storage, &vertexBuffer);
	CustomizableRasterizer<VertexBuffer, OLCDrawing> raster = CustomizableRasterizer<VertexBuffer, OLCDrawing>(&vertexBuffer, &drawer);

	float xshift, yshift;
	float cellSize = 10;
	float timer = 0;

	bool OnUserCreate() override {
		try {
			meshLoader.LoadMesh("testCube.glb", mesh);
		}
		catch (std::exception& exc) {
			printf("exception during loading stage:\n\t");
			printf("%s\n", exc.what());
		}

		mesh.PrintData();

		vertexBuffer.InsertVertex(Vec3(20, 20, 0));
		vertexBuffer.InsertVertex(Vec3(50, 20, 0));
		vertexBuffer.InsertVertex(Vec3(25, 70, 0));


		return true;
	}
	

	bool OnUserUpdate(float dt) override {

		Clear(olc::BLACK);
		xshift += (15.0f * dt + 54.0f*sinf(timer*0.5f)*dt);
		yshift += 10 * dt;
		timer += dt;
		cellSize = 10.0f + 0.25f*sinf(timer*13.0f);

		for (float i = 0; i < ScreenWidth(); i++) {
			for (float j = 0; j < ScreenHeight(); j++) {

				int x = (int)(i + xshift) / cellSize;
				int y = (int)(j + yshift) / cellSize;

				if ( (x+y) % 2 == 0) {
					this->GetDrawTarget()->SetPixel(i, j, olc::Pixel(25, 25, 25));
				}else
					this->GetDrawTarget()->SetPixel(i, j, olc::Pixel(55, 55, 55));
			}
		}

		raster.DrawWires();
		return true;
	}

};

int main() {
	

	try {
		MainC game;
		if (game.Construct(400, 400, 1, 1))
			game.Start();
	}
	catch (std::exception& exc) {
		printf("exception during preparation stage:\n\t");
		printf("%s\n", exc.what());
	}

	std::cout << sizeof(std::vector<float>) << std::endl;
	std::cout << sizeof(std::vector<uint32_t>) << std::endl;


	return 0;
}