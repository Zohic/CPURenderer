#include <iostream>

#include <CPURENDefines.h>

#define CPU_RENDER_IMPLEMENTATION
#include <olcPGEBind.h>
#include <functional>

using namespace cpuRenderBase;

Vec3 lineB = {-2, -2, 0}, lineE = {2, 2, 0};
float angle = 0.0f;
float posZ = 5.5f;
Mat4x4 rot;

RenderInstance* plane;

float elapsedTime = 0.0f;

void start(RenderBackend* back) {
	plane = &back->MakePlane(Vec3(0.0f, 0.0f, 17.0f), Vec2(2.0f, 2.0f));
}

void update(RenderBackend* back) {
	elapsedTime += back->DeltaTime();
	plane->transform.rot.y() += 1.2 * back->DeltaTime();
	
	plane->transform.pos.y() = 6.3f * sinf(elapsedTime * 10.0f);

	/*cpuRenderBase::utils::FillRotationMatrix(Vec3(0, 0, angle), rot);

	angle += 0.8f * back->DeltaTime();

	Vec4 lbl = Vec4(lineB.x(), lineB.y(), lineB.z(), 1.0f);
	Vec4 lel = Vec4(lineE.x(), lineE.y(), lineE.z(), 1.0f);

	lbl = rot * lbl;
	lel = rot * lel;

	lbl.z() += posZ;
	lel.z() += posZ;

	//posZ += 0.25f * back->DeltaTime();

	back->DrawProjectedLine(lbl.block<3, 1>(0, 0), lel.block<3, 1>(0, 0), { 255, 0, 0 });*/
};

int main() {
	std::cout << "void*: " << sizeof(void*) << "\n";
	std::cout << "Vec 2: " << sizeof(Vec2) << "\n";
	std::cout << "Vec 3: " << sizeof(Vec3) << "\n";
	std::cout << "Vec 4: " << sizeof(Vec4) << "\n";
	std::cout << "Mat4x4: " << sizeof(Mat4x4) << "\n";
	olcPGEBind game(start, update);
	game.Run(400, 400, false);

	return 0;
}


