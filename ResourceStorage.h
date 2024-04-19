#pragma once
#include "Render.h"

namespace cpuRenderSimple {
	using namespace cpuRenderBase;
	using namespace std::string_literals;

	class ResourceStorge final: public IResourceStorge {

		std::unordered_map<std::string, std::unique_ptr<MeshData>> meshStorage;
		std::unordered_map<std::string, std::unique_ptr<Material>> materialStorage;
		std::unordered_map<std::string, RenderShape> renderShapes;

		size_t uidCounter = 0;

	public:
		virtual void ReserveMesh(std::string& name) override {
			if (meshStorage.find(name) != meshStorage.end())
				throw std::exception(("a mesh with that name is already reserved: "s + name).c_str());

			meshStorage[name] = nullptr;
		}
		virtual void ReserveMaterial(std::string& name) override {
			if (materialStorage.find(name) != materialStorage.end())
				throw std::exception(("a material with that name is already reserved: "s + name).c_str());

			materialStorage[name] = nullptr;
		}

		virtual MeshData& GetMesh(std::string& name) override {
			if (meshStorage.find(name) == meshStorage.end())
				throw std::exception(("no mesh with such name: "s + name).c_str());
			return *(meshStorage[name]);
		}
		virtual Material& GetMaterial(std::string& name) override {
			if (materialStorage.find(name) == materialStorage.end())
				throw std::exception(("no material with such name: "s + name).c_str());
			return *(materialStorage[name]);
		}

		virtual void RegisterRenderShape(std::string& shapeName, std::string& meshName, std::string& matName) override {
			if (renderShapes.find(shapeName) != renderShapes.end())
				throw std::exception(("a shape with that name already exists: "s + shapeName).c_str()); 

			renderShapes.emplace(shapeName, RenderShape(&GetMesh(meshName), &GetMaterial(matName)));
		}
		virtual const RenderShape& GetShape(std::string& shapeName) const override {
			if (renderShapes.find(shapeName) == renderShapes.end())
				throw std::exception(("no shape with such name: "s + shapeName).c_str());

			return renderShapes.at(shapeName);
		}

	};
}