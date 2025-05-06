#pragma once

#include "../include/Render.h"
#include "../include/ResourceStorage.h"

#include "../include/MeshLoader.h"

#include "../include/VertexBuffer.h"

#define CPU_RENDER_IMPLEMENTATION
#include "../include/Transformer.h"
#include "../include/CustomizableRasterizer.h"

namespace cpuRenderBase {
	using namespace cpuRenderSimple;

	class RenderBackend: public IDrawingFunctional {
	public:

		using BackendFunction = void (*)(RenderBackend* const eng);

	private:
		ResourceStorge storage;
		MeshLoader meshLoader;

		VertexBuffer vertexBuffer;
		Transformer<VertexBuffer> transformer = Transformer<VertexBuffer>(&vertexBuffer);
		CustomizableRasterizer raster = CustomizableRasterizer(&vertexBuffer, static_cast<IDrawingFunctional*>(this));

		std::vector<RenderInstance> ren_instances;

		
	protected:

		void PipeLineProcess() {
			try {
				DEBUGPRINT("main loop transformation begin\n");
				vertexBuffer.ResetIterators(true);
				for (const auto& ren_inst : ren_instances) {
					transformer.ProcessRenderInstance(ren_inst);
				}
			}
			catch (std::exception& exc) {
				printf("Exception during transformation: \n\t%s\n", exc.what());
			}


			try {
				DEBUGPRINT("main loop rasterization begin\n");
				raster.DrawWires();
			}
			catch (std::exception& exc) {
				printf("Exception during rasterization: \n\t%s\n", exc.what());
			}
		}

		const BackendFunction setupFunction;
		const BackendFunction updateFunction;
		float deltaTime;

	public:

		RenderBackend(BackendFunction setupFunc, BackendFunction updateFunc) : setupFunction(setupFunc), updateFunction(updateFunc) {};

		using cstr_t = const std::string&;

		void DrawProjectedLine(const Vec3 p1, const Vec3 p2, gmtl::Vec<uint8_t, 3> color) const {
			Mat4x4 m;
			memcpy(m.mData, transformer.GetProjectionMatrix().mData, sizeof(float) * 16);
		

			Vec4 pp1 = m * Vec4(p1.x(), p1.y(), p1.z(), 1.0f);
			Vec4 pp2 = m * Vec4(p2.x(), p2.y(), p2.z(), 1.0f);

			pp1 /= pp1.w();
			pp2 /= pp2.w();
			
			const float szx = raster.GetViewportSize().x() / 2;
			const float szy = raster.GetViewportSize().y() / 2;
			const float px = szx + raster.GetViewportPos().x();
			const float py = szy + raster.GetViewportPos().y();

			pp1.x() *= szx;
			pp1.y() *= szy;
			pp2.x() *= szx;
			pp2.y() *= szy;

			pp1.x() += px;
			pp1.y() += py;
			pp2.x() += px;
			pp2.y() += py;

			/*Vec3 pp1 = p1 / p1.z() * 100.0f;
			Vec3 pp2 = p2 / p2.z()*100.0f;

			pp1.x() += raster.GetViewportSize().x() / 2;
			pp1.y() += raster.GetViewportSize().y() / 2;

			pp2.x() += raster.GetViewportSize().x() / 2;
			pp2.y() += raster.GetViewportSize().y() / 2;*/

			this->DrawLine(pp1.x(), pp1.y(), pp2.x(), pp2.y(), color.x(), color.y(), color.z());
		}

		void LoadMesh(cstr_t path, cstr_t name) {
			storage.ReserveMesh(name);
			meshLoader.LoadMesh(path.c_str(), storage.GetMesh(name));
		}

		Material& MakeMaterial(cstr_t name, uint8_t attribute_mask) {
			return storage.ReserveMaterial(name, attribute_mask);
		}

		Material& MakeMaterial(cstr_t name, const bool* attribute_list) {
			return storage.ReserveMaterial(name, AttributeAvailability::AttrListToMask(attribute_list));
		}

		void MakeRenderShape(cstr_t name, cstr_t mesh_name, cstr_t material_name) {
			storage.RegisterRenderShape(name, mesh_name, material_name);
		}

		void SetupCamera(float fovY, float aspectRatio, float front, float back) {
			transformer.SetCamera(fovY, aspectRatio, front, back);
		}

		void SetupViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
			raster.SetViewPort(x, y, w, h);
		}

		float DeltaTime() const { return deltaTime; };

		virtual void Run(uint32_t width, uint32_t height, bool fullscreen) = 0;
	};

}

