#include "UnitTest++/Config.h"
#include "UnitTest++/UnitTestPP.h"
#include <iostream>

#include <VertexData.h>
#include <Render.h>
#include <PlaneProcessing.h>

using namespace cpuRenderBase;

SUITE(SUITE_LINE_CLIP) {
	int TestLineClip(const char* name, Vec3 p1, Vec3 p2, Vec4 plane, Vec4 expected) {
	try {
		VertexData v1;
		VertexData v2;
		bool al[5]{ true, true, false, false, false };
		al[ATTR_NORMAL_INDEX] = true;
		al[ATTR_COLOR_INDEX] = true;
		v1.Init(al);
		v2.Init(al);
		//delete[] al;

		v1.SetPos(p1);
		v2.SetPos(p2);

		cpuRenderBase::utils::InterpolateToPlane(plane, v1, v2, al);

		if (v1.GetPos() == expected)
			return 0;
		else
			return 1;

	}
	catch (std::exception& e) {
		std::cout << "\terror during " << name << " test: " << e.what() << "\n";
		return 2;
	}
}


TEST(LINE_CLIPS) {
	
	/*CHECK(0 == TestLineClip("X CLIP",           Vec3(-5, 0, 0),  Vec3(10, 0, 0),  Vec4(1, 0, 0, -1),    Vec4(-1, 0, 0, 1)    ));
	CHECK(0 == TestLineClip("NEG X CLIP",       Vec3(10, 0, 0),  Vec3(-5, 0, 0),  Vec4(1, 0, 0, -1),    Vec4(-1, 0, 0, 1)    ));
	CHECK(0 == TestLineClip("Y CLIP",           Vec3(0, 1, 0),   Vec3(0, -1, 0),  Vec4(0, -1, 0, 0.3f), Vec4(0, -0.3f, 0, 1) ));
	CHECK(0 == TestLineClip("Y CLIP OTHER WAY", Vec3(0, 1, 0),   Vec3(0, -1, 0),  Vec4(0, 1, 0, 0.3f),  Vec4(0, 0.3f, 0, 1)  ));
	CHECK(0 == TestLineClip("Z CLIP",           Vec3(0, 0, 10),  Vec3(0, 0, -10), Vec4(0, 0, -1, 1.3f), Vec4(0, 0, -1.3f, 1) ));
	CHECK(0 == TestLineClip("DIAG CLIP",        Vec3(0, -5, -3), Vec3(0, 5, 3),   Vec4(0, 1, 0, 0),     Vec4(0, 0, 0, 1)     ));
	CHECK(0 == TestLineClip("SHIFT CLIP",       Vec3(0, 5, -3),  Vec3(0, 5, 3),   Vec4(0, 0, 1, 0),     Vec4(0, 5, 0, 1)     ));
	*/
}
}






