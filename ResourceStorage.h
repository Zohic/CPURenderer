#pragma once
#include "Render.h"

namespace cpuRenderSimple {
	using namespace cpuRenderBase;
	using namespace std::string_literals;

	class ResourceStorge {

		std::unordered_map<std::string, std::unique_ptr<MeshData>> meshStorage;
		std::unordered_map<std::string, std::unique_ptr<Material>> materialStorage;
		std::unordered_map<std::string, RenderShape> renderShapes;

		size_t uidCounter = 0;

		ResourceStorge(const ResourceStorge&) = delete;
		ResourceStorge(ResourceStorge&&) = delete;

	public:

		ResourceStorge() = default;


		void ReserveMesh(const std::string& name) {
			if (meshStorage.find(name) != meshStorage.end())
				throw std::exception(("a mesh with that name is already reserved: "s + name).c_str());

			meshStorage[name].reset(new MeshData());
		}
		void ReserveMaterial(const std::string& name, uint8_t mask) {
			if (materialStorage.find(name) != materialStorage.end())
				throw std::exception(("a material with that name is already reserved: "s + name).c_str());

			materialStorage[name].reset(new Material(mask));
		}

		MeshData& GetMesh(const std::string& name) {
			if (meshStorage.find(name) == meshStorage.end())
				throw std::exception(("no mesh with such name: "s + name).c_str());
			return *(meshStorage[name]);
		}
		Material& GetMaterial(const std::string& name) {
			if (materialStorage.find(name) == materialStorage.end())
				throw std::exception(("no material with such name: "s + name).c_str());
			return *(materialStorage[name]);
		}

		void RegisterRenderShape(const std::string& shapeName, const std::string& meshName, const std::string& matName) {
			if (renderShapes.find(shapeName) != renderShapes.end())
				throw std::exception(("a shape with that name already exists: "s + shapeName).c_str()); 

			renderShapes.emplace(shapeName, RenderShape(&GetMesh(meshName), &GetMaterial(matName)));
		}
		const RenderShape& GetShape(const std::string& shapeName) const {
			if (renderShapes.find(shapeName) == renderShapes.end())
				throw std::exception(("no shape with such name: "s + shapeName).c_str());

			return renderShapes.at(shapeName);
		}

	};
}