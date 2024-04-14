//#include "Render.h"
//#include "MeshLoader.h"
#include <iostream>
#include "Zohich3DEngine.h"

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
	VertexPipeLine pipeLine;
	Renderer renderer;

	VertexBuffer mainVertexBuffer;
	FrameBuffer screenBuffer;

	storage.RegisterMesh("coin mesh", meshLoader.Load("coin.gltf));

	const Material normalMat(
		ATTR_NORMAL|ATTR_TEXCOORD,
		[](const &VertexShaderInput input) {
			return input.vertexData.position * input.objectData.worldMatrix * input.sceneData.viewMatrix;
		},
		[](const PixelShaderInput &input) {
			return Vec4(input.attributes.normal.x, input.attributes.normal.y, input.attributes.normal.z, 1.0);
		}
	);

	storage.RegisterMaterial("coin material", &normalMat);

	std::vector<RenderObjectRef> coins(10);

	for(int i=0; i < 10; i++) {
		coins.push_back(new RenderObject(Vec3(i*5, 0, 0), storage.GetMesh("coin mesh"), storage.GetMaterial("normal material")));
	}

	pipeLine.AddObjects(coins.begin(), coins.end());

	pipeLine.SetVertexBuffer(&mainVertexBuffer);

	pipeLine.SetVertexBuffer(&mainVertexBuffer);
	renderer.SetFrameBuffer(&screenBuffer);

	pipeLine.FillVertexBuffer();
	rendere.FillFrameBuffer();

*/



int main() {
	/*cpuRenderA::MeshData mesh; 
	if (!mesh.LoadGLTF("testCube.glb")) {
		mesh.PrintData();
	}*/	
	return 0;
}
