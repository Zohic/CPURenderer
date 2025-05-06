#include "RenderInstance.h"

using namespace cpuRenderBase;

RenderInstance::RenderInstance(const RenderShape* ishapeID) : transform(), shape(const_cast<RenderShape*>(ishapeID)) {

}
RenderInstance::RenderInstance(Transform itrans, const RenderShape* ishapeID) : transform(itrans), shape(const_cast<RenderShape*>(ishapeID)) {

}

RenderInstance::RenderInstance(const RenderInstance& other) : transform(other.transform), shape(other.shape) {

}

RenderInstance& RenderInstance::operator=(const RenderInstance& other) {
	transform = other.transform;
	shape = other.shape;
	return *this;
}

const RenderShape* RenderInstance::GetShape() const {
	return shape;
}