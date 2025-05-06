#include "Material.h"
using namespace cpuRenderBase;

Material::Material(uint8_t mask) : UniformStorage(), AttributeAvailability(mask) {

}

Material::Material(const bool* list) : UniformStorage(), AttributeAvailability(list) {

}

Material& Material::SetVertexShader(const VertexShader vs) {
	if (vs == nullptr)
		throw std::logic_error("setting vertex shader to null");
	vertexShader = vs;

	return *this;
}
Material& Material::SetFragmentShader(const FragmentShader fs) {
	if (fs == nullptr)
		throw std::logic_error("setting fragment shader to null");
	fragmentShader = fs;

	return *this;
}

const VertexShader Material::GetVertexShader() const {
	if (!vertexShader)
		throw std::logic_error("trying to access vertex shader that is not existent");
	return vertexShader;
}
const FragmentShader Material::GetFragmentShader() const {
	if (!vertexShader)
		throw std::logic_error("trying to access vertex shader that is not existent");
	return fragmentShader;
}


const UniformStorage* Material::GetUniforms() const {
	return this;
}