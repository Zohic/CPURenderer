#pragma once


//#define CPUREN_DEBUG

#include "Render.h"

using namespace cpuRenderBase;

#define PrintFail(text, ...)\
{ SetConsoleTextAttribute(hConsole, FOREGROUND_RED);\
  printf(text, __VA_ARGS__);\
  SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);\
}\

#define PrintSuccess(text, ...)\
{ SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);\
  printf(text, __VA_ARGS__);\
  SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);\
}\

void MakeTest(const bool cond, const char* name) {
	if (cond) {
		PrintSuccess("\t TEST: %s PASSED\n", name)
	}
	else {
		PrintFail("\t TEST: %s FAILED\n", name);
	}
		
}

void MakeTestFloatEq(const float f1, const float f2, const char* name) {
	MakeTest(fabsf(f1 - f2) < 0.0001f, name);
}

template<uint32_t dim>
void MakeTestVecEq(const gmtl::Vec<float, dim>& v1, const gmtl::Vec<float, dim>& v2, const char* name) {
	MakeTestFloatEq(gmtl::length<float, dim>(v1 - v2), 0.0f, name);
}



/*#define TESTPRINT(cond, name)\
if(cond)\
		PrintSuccess("\t TEST: %s PASSED\n", name) \
else\
		PrintError("\t TEST: %s FAILED\n", name); \

#define TEST_FLOAT_EQ(f1, f2, name) TESTPRINT(fabsf((f1) - (f2)) < 0.001f, name)

#define TEST_VEC4_EQ(v1, v2, name) TEST_FLOAT_EQ((gmtl::length<float, 4>(v1 - v2)), 0.0f, name)*/

void TestVertexData() {
	try {
		
	}
	catch (std::exception& e) {
		std::cout << "\terror during TEST VERTEX DATA" << " test: " << e.what() << "\n";
	}
}

void TestTriangleWrap() {
	try {
		
	}
	catch (std::exception& e) {
		std::cout << "\terror during TEST TRIANGLE WRAP" << " test: " << e.what() << "\n";
	}
	
}

void TestLineClip(const char* name, Vec3 p1, Vec3 p2, Vec4 plane, Vec4 expected) {
	try {
		VertexData v1;
		VertexData v2;
		bool al[5]{ true, true, false, false, false };
		al[ATTR_NORMAL] = true;
		al[ATTR_COLOR] = true;
		v1.Init(al);
		v2.Init(al);
		//delete[] al;

		v1.SetPos(p1);
		v2.SetPos(p2);

		cpuRenderBase::utils::InterpolateToPlane(plane, v1, v2, al);

		MakeTestVecEq<4>(v1.GetPos(), expected, name);

		//TEST_VEC4_EQ(v1.GetPos(), expected, name.c_str());

	}
	catch (std::exception& e) {
		std::cout << "\terror during " << name << " test: " << e.what() << "\n";
		//printf("\terror during %s test: %s\n", name.c_str(), e.what()); 
	}

}

void TestLineClips() {
	TestLineClip("X CLIP", Vec3(-5, 0, 0), Vec3(10, 0, 0), Vec4(1, 0, 0, -1), Vec4(-1, 0, 0, 1));
	TestLineClip("NEG X CLIP", Vec3(10, 0, 0), Vec3(-5, 0, 0), Vec4(1, 0, 0, -1), Vec4(-1, 0, 0, 1));
	TestLineClip("Y CLIP", Vec3(0, 1, 0), Vec3(0, -1, 0), Vec4(0, -1, 0, 0.3f), Vec4(0, -0.3f, 0, 1));
	TestLineClip("Y CLIP OTHER WAY", Vec3(0, 1, 0), Vec3(0, -1, 0), Vec4(0, 1, 0, 0.3f), Vec4(0, 0.3f, 0, 1));
	TestLineClip("Z CLIP", Vec3(0, 0, 10), Vec3(0, 0, -10), Vec4(0, 0, -1, 1.3f), Vec4(0, 0, -1.3f, 1));
	TestLineClip("DIAG CLIP", Vec3(0, -5, -3), Vec3(0, 5, 3), Vec4(0, 1, 0, 0), Vec4(0, 0, 0, 1));
	TestLineClip("SHIFT CLIP", Vec3(0, 5, -3), Vec3(0, 5, 3), Vec4(0, 0, 1, 0), Vec4(0, 5, 0, 1));
}

void TestTriagClip(const char* name, Vec3 p1, Vec3 p2, Vec3 p3, Vec4 plane, int expected) {
	try {
		VertexData* vs = new VertexData[3];


		bool al[5]{ true, false, false, false, false };
		al[ATTR_NORMAL] = true;
		al[ATTR_COLOR] = true;
		vs[0].Init(al);
		vs[1].Init(al);
		vs[2].Init(al);
		//delete[] al;

		vs[0].SetPos(p1);
		vs[1].SetPos(p2);
		vs[2].SetPos(p3);


		std::vector<TriangleWrap> ts;
		ts.reserve(3);
		ts.emplace_back(vs);

		int a = cpuRenderBase::utils::ClipAgainstPlane(plane, 0, ts, al);

		//cpuRenderBase::utils::InterpolateToPlane(plane, v1, v2, al);

		//MakeTestVecEq<4>(v1.GetPos(), expected, name);
		MakeTest(a == expected, name);
		
		for (auto& t : ts)
			t = std::move(TriangleWrap((VertexData*)nullptr));

		//TEST_VEC4_EQ(v1.GetPos(), expected, name.c_str());

	}
	catch (std::exception& e) {
		std::cout << "\terror during " << name << " test: " << e.what() << "\n";
		//printf("\terror during %s test: %s\n", name.c_str(), e.what()); 
	}

}

void TestCPUREN() {
	std::cout << "TESTS: \n";

	MakeTestVecEq<2>(Vec2(1.0f, 0.0f), Vec2(1.0f, 1.0f) - Vec2(0.0f, 1.0f), "vec2 equal");
	MakeTestVecEq<3>(Vec3(0.0f, 0.0f, 0.5f), Vec3(2.4f, -20000.0f, 5.85f) + Vec3(-2.4f, 20000.0f, -5.35f), "vec3 equal");
	MakeTestVecEq<4>(Vec4(12.4f, 100.0f / 3.0f, 44.0f, 999.75f), Vec4(12.4f, 300.0f / 9.0f, 44.0f, 333.25f * 3), "vec4 equal");

	printf("---VertexData---\n");
	TestVertexData();

	printf("---TriangleWrap---\n");
	TestTriangleWrap();

	printf("---LineClip---\n");
	TestLineClips();

	printf("---TriagClip---\n");
	TestTriagClip("TRIG CLIP",   Vec3(-5, 3, 0), Vec3(-5, -3, 0), Vec3(5, 3, 0), Vec4(0, 1, 0, 0), 2);
	TestTriagClip("TRIG CLIP 2", Vec3(-5, 3, 0), Vec3(-5, -3, 0), Vec3(5, 3, 0), Vec4(0, -1, 0, 0), 1);
}
