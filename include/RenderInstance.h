#pragma once
#include "Transform.h"
#include "RenderShape.h"

namespace cpuRenderBase {
	class RenderInstance {
	public:
		Transform transform; //public since any value is valid
	private:
		RenderShape* shape;

		RenderInstance() = delete;
		//makes no sence to move objects of this class
	public:
		RenderInstance(const RenderShape* ishapeID);
		RenderInstance(Transform itrans, const RenderShape* ishapeID);

		RenderInstance(const RenderInstance& other);

		RenderInstance& operator=(const RenderInstance& other);

		const RenderShape* GetShape() const;
	};
}