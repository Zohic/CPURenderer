#pragma once

#include "UniformStorage.h"

namespace cpuRenderBase {

	class LightSource;

	class VertexShaderInput {
	public:
		Vec3 cameraPosition;
		Mat4x4 projectionMatrix;
		//80
		Vec3 cameraDirection;
		Mat4x4 viewMatrix;
		//80


		Mat4x4 translationMatrix;
		Mat4x4 scalingMatrix;
		Mat4x4 rotationMatrix;
		Mat4x4 worldMatrix;

		std::vector<LightSource> lights;
		UniformStorage* uniforms = nullptr;

		const bool* availableAttrs = nullptr;

	};
}