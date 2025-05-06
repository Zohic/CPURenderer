#pragma once
#include "Render.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace cpuRenderSimple {
	using namespace cpuRenderBase;
	using namespace std::string_literals;


	class ResourceStorge {

		using MeshPtr = std::unique_ptr<Mesh>;
		using MaterialPtr = std::unique_ptr<Material>;

		template<typename K, typename V>
		using HashMap = std::unordered_map<K, V>;

		HashMap<std::string, MeshPtr> meshStorage;
		HashMap<std::string, MaterialPtr> materialStorage;
		HashMap<std::string, RenderShape> renderShapes;

		size_t uidCounter = 0;

		ResourceStorge(const ResourceStorge&) = delete;
		ResourceStorge(ResourceStorge&&) = delete;

	public:
#define C_STR(V) (V).c_str()


		ResourceStorge() = default;

		//
		void ReserveMesh(const std::string& name) {
			if (meshStorage.find(name) != meshStorage.end())
				throw std::logic_error(C_STR("a mesh with that name is already reserved: "s + name));

			meshStorage[name].reset(new Mesh());
		}
		Material& ReserveMaterial(const std::string& name, uint8_t mask) {
			if (materialStorage.find(name) != materialStorage.end())
				throw std::logic_error(("a material with that name is already reserved: "s + name).c_str());

			materialStorage[name].reset(new Material(mask));
			return *(materialStorage[name]);
		}

		Mesh& GetMesh(const std::string& name) {
			if (meshStorage.find(name) == meshStorage.end())
				throw std::logic_error(("no mesh with such name: "s + name).c_str());
			return *(meshStorage[name]);
		}
		Material& GetMaterial(const std::string& name) {
			if (materialStorage.find(name) == materialStorage.end())
				throw std::logic_error(("no material with such name: "s + name).c_str());
			return *(materialStorage[name]);
		}

		void RegisterRenderShape(const std::string& shapeName, const std::string& meshName, const std::string& matName) {
			if (renderShapes.find(shapeName) != renderShapes.end())
				throw std::logic_error(("a shape with that name already exists: "s + shapeName).c_str()); 

			renderShapes.emplace(shapeName, RenderShape(&GetMesh(meshName), &GetMaterial(matName)));
		}
		const RenderShape& GetShape(const std::string& shapeName) const {
			if (renderShapes.find(shapeName) == renderShapes.end())
				throw std::logic_error(("no shape with such name: "s + shapeName).c_str());

			return renderShapes.at(shapeName);
		}

	};
}