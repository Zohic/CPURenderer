#pragma once
#include <stdint.h>
#include <cstring>

#ifndef DEBUGPRINT
#ifdef CPUREN_DEBUG
#define DEBUGPRINT(text, ...) printf(text, __VA_ARGS__);
#else
#define DEBUGPRINT(text, ...) ;
#endif // !CPUREN_DEBUG
#endif // !DEBUGPRINT

#include "gmtl/gmtl.h"
#include <string>


namespace cpuRenderBase {

	static std::string operator+(const std::string& s, int n) {
		return s + std::to_string(n);
	}

	

	using Vec2 = gmtl::Vec2f;
	using Vec3 = gmtl::Vec3f;
	using Vec4 = gmtl::Vec4f;
	using Mat4x4 = gmtl::Matrix44f;

	constexpr float PI = 3.14159265f;
	constexpr float PI_2 = PI * 0.5f;

	constexpr size_t ATTR_POS_INDEX = 0ULL;
	constexpr size_t ATTR_NORMAL_INDEX = 1ULL;
	constexpr size_t ATTR_TANGENT_INDEX = 2ULL;
	constexpr size_t ATTR_TEXCOORD_INDEX = 3ULL;
	constexpr size_t ATTR_COLOR_INDEX = 4ULL;


	constexpr size_t ATTRIBUTES_NUM = 5ULL;

	constexpr uint8_t ATTR_POS_MASK = 1;
	constexpr uint8_t ATTR_NORMAL_MASK = 2;
	constexpr uint8_t ATTR_TANGENT_MASK = 4;
	constexpr uint8_t ATTR_TEXCOORD_MASK = 8;
	constexpr uint8_t ATTR_COLOR_MASK = 16;

	constexpr bool ATTR_PN[5] = { true, true, false, false, false };
	constexpr bool ATTR_PNTX[5] = { true, true, false, true, false };
	constexpr bool ATTR_PNC[5] = { true, true, false, false, true };
	constexpr bool ATTR_PNTXC[5] = { true, true, false, true, true };

	constexpr uint8_t atrr_offsets[5] = {
	ATTR_POS_MASK,
	ATTR_NORMAL_MASK,
	ATTR_TANGENT_MASK,
	ATTR_TEXCOORD_MASK,
	ATTR_COLOR_MASK };

}

