#include <iostream>

#include <CPURENDefines.h>

#define CPU_RENDER_IMPLEMENTATION
#include <olcPGEBind.h>
#include <functional>

using namespace cpuRenderBase;

Vec3 lineB = {-2, -2, 0}, lineE = {2, 2, 0};
float angle = 0.0f;
float posZ = 2.5f;
Mat4x4 rot;

void start(RenderBackend* back) {
	
}

void update(RenderBackend* back) {
	cpuRenderBase::utils::FillRotationMatrix(Vec3(angle, 0, 0), rot);

	angle += 0.8f * back->DeltaTime();

	Vec3 lbl = lineB;
	Vec3 lel = lineE;

	lbl = rot * lbl;
	lel = rot * lel;

	lbl.z() += posZ;
	lel.z() += posZ;

	posZ += 0.25f * back->DeltaTime();

	back->DrawProjectedLine(lbl, lel, { 255, 0, 0 });
};

int main() {
	
	olcPGEBind game(start, update);
	game.Run(400, 400, false);

	return 0;
}


