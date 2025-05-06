#include "UnitTest++/Config.h"
#include "UnitTest++/UnitTestPP.h"

#include <VertexData.h>
#include <Material.h>

using namespace cpuRenderBase;

SUITE(SUITE_VERTEX_DATA) {
	TEST(PUSHING_TEST) {
		{
			Material mat(ATTR_POS_MASK | ATTR_NORMAL_MASK | ATTR_COLOR_MASK);
			std::vector<VertexData> verts_from_temp;
			std::vector<VertexData> verts_from_glob;
			std::vector<VertexData> verts_from_both;
			VertexData vd;

			const auto InitVert = [&mat](VertexData& vert, int i) {
				vert.Init(mat.GetAttrList());
				vert.SetPos(Vec3(1 * i, 2 * i, 3 * i));
				vert.SetNormal(Vec3(-1 * i, -3 * i, -5 * i));
				vert.SetColor(Vec4(i, 1 + i, 2 + i, 3 + i), mat.GetAttrList());
			};

			for (int i = 0; i < 50; i++)
			{

				{
					VertexData newVert;
					InitVert(newVert, i);
					verts_from_temp.push_back(std::move(newVert));
				}

				vd = VertexData();
				InitVert(vd, i);
				verts_from_glob.push_back(std::move(vd));

				if (i % 2 == 0) {
					vd = VertexData();
					InitVert(vd, i);
					verts_from_both.push_back(std::move(vd));
				}
				else {
					VertexData newVert;
					InitVert(newVert, i);
					verts_from_both.push_back(std::move(newVert));
				}
			}

			CHECK(verts_from_temp.size() == 50);
			CHECK(verts_from_glob.size() == 50);
			CHECK(verts_from_both.size() == 50);

			for (int i = 0; i < 50; i++)
			{
				CHECK(verts_from_temp[i].GetPos() == Vec4(1 * i, 2 * i, 3 * i, 1));
				CHECK(verts_from_glob[i].GetPos() == Vec4(1 * i, 2 * i, 3 * i, 1));
				CHECK(verts_from_both[i].GetPos() == Vec4(1 * i, 2 * i, 3 * i, 1));

				CHECK(verts_from_temp[i].GetNormal() == Vec3(-1 * i, -3 * i, -5 * i));
				CHECK(verts_from_glob[i].GetNormal() == Vec3(-1 * i, -3 * i, -5 * i));
				CHECK(verts_from_both[i].GetNormal() == Vec3(-1 * i, -3 * i, -5 * i));

				CHECK(verts_from_temp[i].GetColor(mat.GetAttrList()) == Vec4(i, 1 + i, 2 + i, 3 + i));
				CHECK(verts_from_glob[i].GetColor(mat.GetAttrList()) == Vec4(i, 1 + i, 2 + i, 3 + i));
				CHECK(verts_from_both[i].GetColor(mat.GetAttrList()) == Vec4(i, 1 + i, 2 + i, 3 + i));
			}

		}
	}

	TEST(TEST_WITH_VECTOR) {
		std::vector<VertexData> verts;

		Material mat(ATTR_POS_MASK | ATTR_NORMAL_MASK | ATTR_COLOR_MASK);
		const bool* attrList = mat.GetAttrList();

		verts.push_back(VertexData());
		verts.emplace_back();

		{
			VertexData v{};
			verts.emplace_back(std::move(v));
		}

		verts.reserve(5);
		verts.resize(10);



		for (auto& vert : verts) {
			vert.Init(attrList);
			CHECK_EQUAL(vert.GetPos(), Vec4(0, 0, 0, 0));
			CHECK_EQUAL(vert.GetNormal(), Vec3(0, 0, 0));
			CHECK_EQUAL(vert.GetColor(attrList), Vec4(0, 0, 0, 0));
		}

		verts[2].SetPos(Vec3(2, 3, 5));
		verts[2].SetNormal(Vec3(1, 1.2, 1));
		verts[2].SetColor(Vec4(2.2, 3.3, 4.4, 5.5), attrList);

		CHECK_EQUAL(verts[2].GetPos(), Vec4(2, 3, 5, 1));
		CHECK_EQUAL(verts[2].GetNormal(), Vec3(1, 1.2, 1));
		CHECK_EQUAL(verts[2].GetColor(attrList), Vec4(2.2, 3.3, 4.4, 5.5));
	}
}
