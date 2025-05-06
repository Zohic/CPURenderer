#include "UnitTest++/Config.h"
#include "UnitTest++/UnitTestPP.h"

#include "VertexBuffer.h"
#include <iostream>
using namespace cpuRenderBase;
using namespace cpuRenderSimple;

SUITE(SUITE_VERTEX_BUFFER) {
	void FillVertexBuffer(VertexBuffer & vb, const Material & mat, int count) {

		try {
			for (int i = 0; i < count; i++) {
				VertexData vd = VertexData();
				vd.Init(mat.GetAttrList());

				vd.SetPos(Vec3(1 * i, 2 * i, 3 * i));
				vd.SetNormal(Vec3(-1 * i, -3 * i, -5 * i));
				vd.SetColor(Vec4(i, 1 + i, 2 + i, 3 + i), mat.GetAttrList());

				vb.InsertVertex(std::move(vd), &mat);
			}
		}
		catch (std::exception& exc) {
			std::cout << exc.what() << '\n';
		}
	}

	TEST(TEST_ADDING_LESS) {
		VertexBuffer vb;
		Material mat(ATTR_POS_MASK | ATTR_NORMAL_MASK | ATTR_COLOR_MASK);

		FillVertexBuffer(vb, mat, 20);
		CHECK(vb.GetVertexCount() == 20);

		vb.ResetIterators(false);
		CHECK(vb.GetVertexCount() == 20);

		vb.ResetIterators(true);
		CHECK(vb.GetVertexCount() == 20);
	}
}

