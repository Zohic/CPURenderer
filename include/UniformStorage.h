#pragma once
#include <variant>
#include <string>
#include <unordered_map>
#include <memory>

#include "CPURENDefines.h"

namespace cpuRenderBase {
	
	using UniformValue = std::variant<float, Vec2*, Vec3*, Vec4*, Mat4x4*>;

	class UniformStorage {
	protected:
		std::unordered_map<std::string, UniformValue> uniforms;
	public:
		template<typename T>
		const T GetUniform(std::string& name) const {
			if (!std::holds_alternative<T>(uniforms.at(name)))
				throw std::logic_error("no uniform of such type or name (GET)");

			return std::get<T>(uniforms.at(name));
		}

		template<typename T>
		const T GetUniform(std::string&& name) const {
			if (!std::holds_alternative<T>(uniforms.at(name)))
				throw std::logic_error("no uniform of such type or name (GET)");

			return std::get<T>(uniforms.at(name));
		}

		template<typename T>
		UniformStorage& SetUniform(std::string& name, T v) {
			uniforms[name] = v;
			return *this;
		}

		template<typename T>
		UniformStorage& SetUniform(std::string&& name, T v) {
			uniforms[name] = v;
			return *this;
		}

		void PrintUniforms() const{
			printf("list of unifrom values:\n");

			for (auto uni = uniforms.cbegin(); uni != uniforms.cend(); uni++) {


				const std::string& name = (*uni).first;

				const UniformValue& value = (*uni).second;

				if (std::holds_alternative<float>(value)) {
					printf("\t%s = %f\n", name.c_str(), std::get<float>(value));
				}
				else if (std::holds_alternative<Vec2*>(value)) {
					printf("\t%s = (%f, %f)\n", name.c_str(), std::get<Vec2*>(value)->x(), std::get<Vec2*>(value)->y());
				}
				else if (std::holds_alternative<Vec3*>(value)) {
					printf("\t%s = (%f, %f, %f)\n", name.c_str(), std::get<Vec3*>(value)->x(), std::get<Vec3*>(value)->y(), std::get<Vec3*>(value)->z());
				}
				else if (std::holds_alternative<Vec4*>(value)) {
					printf("\t%s = (%f, %f, %f, %f)\n", name.c_str(),
						std::get<Vec4*>(value)->x(), std::get<Vec4*>(value)->y(), std::get<Vec4*>(value)->z(), std::get<Vec4*>(value)->w());
				}
				else if (std::holds_alternative<Mat4x4*>(value)) {
					printf("\t%s = matrix4x4\n", name.c_str());
				}


			}
		}
	};
}
