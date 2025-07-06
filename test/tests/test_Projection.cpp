#include "UnitTest++/Config.h"
#include "UnitTest++/UnitTestPP.h"
#include <numeric>

#include <Render.h>
#include <Camera.h>

using namespace cpuRenderBase;

SUITE(TEST_PROJ) {

	void test_frustum(float fov, float aspect, float near_p, float far_p) {
		Camera cam;
		cam.SetFrustum(fov, aspect, near_p, far_p);
		constexpr float DEG2RAD = 3.14159265f / 180.f;
		const float eps = std::numeric_limits<float>::epsilon() * 250;

		const Mat4x4 proj_mat = cam.GetProjectionMatrix();

		auto project = [&proj_mat](const Vec4& p) -> Vec4 {
			return proj_mat * p;
		};
		Vec4 point, proj, NDC;

#define MAKE_T(x, y, z)\
		point = Vec4(x, y, z, 1.0f);\
		proj = project(point);\
		NDC = proj / proj.w();\

#define CHECK_CLOSE_EPS(a, b) CHECK_CLOSE(a, b, eps)
#define CHECK_Z do{\
CHECK_CLOSE(NDC.w(), 1.0f, eps);\
CHECK_CLOSE(point.z(), proj.w(), eps);}while(false)\

#define GET_RAND(a, b) (float)a + (float)rand()/RAND_MAX * ((float)b - (float)a)

		MAKE_T(GET_RAND(-10, 10), GET_RAND(-10, 10), far_p);
		CHECK_Z;
		CHECK_CLOSE_EPS(1.0f, NDC.z());

		MAKE_T(GET_RAND(-10, 10), GET_RAND(-10, 10), near_p);
		CHECK_Z;
		CHECK_CLOSE_EPS(0.0f, NDC.z());


		MAKE_T(1.2f, near_p * 4.6f, near_p);
		CHECK_Z;
		CHECK_CLOSE_EPS(1.2f / tan(fov * 0.5f * DEG2RAD), proj.x());
		CHECK_CLOSE_EPS(1.2f / tan(fov * 0.5f * DEG2RAD) / point.z(), NDC.x());
		//CHECK_CLOSE_EPS(4.6f * aspect, NDC.y());
		CHECK_CLOSE_EPS(0.0f, NDC.z());
	}

	TEST(EDGES) {
		test_frustum(75.0f,  1.0f, 0.1f, 1000.0f);
		test_frustum(90.0f,  1.0f, 0.1f, 1000.0f);
		test_frustum(120.0f, 1.0f, 0.1f, 1000.0f);
			
		test_frustum(100.0f, 1.0f, 0.0001f, 1000.0f);
		test_frustum(100.0f, 1.0f, 0.1f,    1000.0f);
		test_frustum(100.0f, 1.0f, 0.5f,    1000.0f);
		
						    
		test_frustum(100.0f, 1.0f, 0.01f, 100.0f);
		test_frustum(100.0f, 1.0f, 0.01f, 10000.0f);
		test_frustum(100.0f, 1.0f, 0.01f, 1000000.0f);

		test_frustum(100.0f, 0.8f, 0.01f, 10000.0f);
		test_frustum(100.0f, 1.1f, 0.01f, 10000.0f);
		test_frustum(100.0f, 1.6f, 0.01f, 10000.0f);

		/*test_frustum(GET_RAND(30.0f, 180.0f), 
					 GET_RAND(0.8f, 1.9f), 
					 GET_RAND(0.001f, 0.1f), 
					 GET_RAND(100.0f, 1000000.0f));
		test_frustum(GET_RAND(30.0f, 180.0f),
			         GET_RAND(0.8f, 1.9f),
			         GET_RAND(0.001f, 0.1f),
			         GET_RAND(100.0f, 1000000.0f));
		test_frustum(GET_RAND(30.0f, 180.0f),
			         GET_RAND(0.8f, 1.9f),
			         GET_RAND(0.001f, 0.1f),
			         GET_RAND(100.0f, 1000000.0f));*/
	}
}