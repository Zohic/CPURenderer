#pragma once

#include "VertexData.h"

#include "AttributeAvailability.h"
#include "UniformStorage.h"

#include "VertexShaderInput.h"

namespace cpuRenderBase {

	typedef void (*VertexShader)(VertexData& v_in, VertexShaderInput& inp);
	typedef Vec4(*FragmentShader)(const VertexData& f_in);

	class Material : public UniformStorage, public AttributeAvailability {

		VertexShader vertexShader = nullptr;
		FragmentShader fragmentShader = nullptr;

		Material() = delete;
		Material(const Material& other) = delete;
		Material(Material&& old) = delete;

		Material& operator=(const Material& other) = delete;
		Material& operator=(Material&& old) = delete;

	public:


		Material(uint8_t mask);

		Material(const bool* list);

		Material& SetVertexShader(const VertexShader vs);
		Material& SetFragmentShader(const FragmentShader fs);

		const VertexShader GetVertexShader() const;
		const FragmentShader GetFragmentShader() const;


		const UniformStorage* GetUniforms() const;
	};
}