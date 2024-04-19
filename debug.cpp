//#include "Render.h"
//#include "MeshLoader.h"
#include <iostream>
#include "Zohich3DEngine.h"
#include "MeshLoader.h"

/*

Data containers:
-MeshData: stores vertrex data (aka attributes);
-Materail: stores global uniforms and links to shaders;
-RenderObject: stores position, orientation and scale alongside with links to mesh and material;
-VertexBuffer: buffer for 
-FrameBuffer: pixel grid with customizable size and depth (might be stencil or depth buffer)

Interfaces:
-IMeshLoader: Loads MeshData from file
	-MeshLoader:  No other options than loadig from .obj and .gltf/.glb files are planned, thus this class is created as standart and final solution
		(Singleton maybe?)

-IVertexPipeLine: Converts list of RenderObjects to vertex buffer that can be processed by renderer

-IRenderer: Renders vertex buffer into framebuffer

-IResourceStorage: must provide convinient way of storing reusable resources like meshes, textures and materials
	-ResourceStorage: name to object reference container
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



int main() {
	cpuRenderSimple::MeshLoader meshLoader; 
	cpuRenderBase::MeshData mesh;

	if (meshLoader.LoadMesh("testCube.glb", mesh) == cpuRenderBase::ErrorCode::OK) {
		mesh.PrintData();
	}

	std::cout << sizeof(std::vector<float>) << std::endl;
	std::cout << sizeof(std::vector<uint32_t>) << std::endl;

	return 0;
}