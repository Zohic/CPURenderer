#include "UnitTest++/Config.h"
#include "UnitTest++/UnitTestPP.h"

#include <TriangleWrap.h>
#include <Material.h>

using namespace cpuRenderBase;

SUITE(SUITE_TRIANGLE_WRAP) {
	TEST(TEST_TRIANGLE_WRAP) {
		Material mat(cpuRenderBase::ATTR_PNTX);
		const bool* attrList = mat.GetAttrList();

		VertexData* v1 = new VertexData[3];
		VertexData* v2 = new VertexData[3];

		std::vector<TriangleWrap> ts;

		TriangleWrap t1(v1);

		ts.push_back(std::move(t1));
		ts.push_back(std::move(t1));
		ts.emplace_back(v1);

		ts[0][0].Init(attrList);

		CHECK_EQUAL(ts[0][0].GetPos(), Vec4(0, 0, 0, 0));
		CHECK_EQUAL(ts[0][0].GetPos(), v1->GetPos());

		struct ttmp {
			VertexData* p;
		};
		CHECK(ts[1] == t1);
		CHECK(reinterpret_cast<ttmp*>(&(ts[2]))->p == v1);

		ts[0] = TriangleWrap((VertexData*)nullptr);
		ts[2] = TriangleWrap((VertexData*)nullptr);
	}
}


