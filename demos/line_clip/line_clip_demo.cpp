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

olcPGEBind* olcBack;

RenderInstance* plane[6];
RenderInstance* frustmObj, *katanaObj;
float elapsedTime = 0.0f;

static const float fov_ang = 90.0f;
static const float far_z = 1000.0f;
static const float near_z = 0.01f;
static const float cam_aspect = 1;


void start(RenderBackend* back) {
	back->SetupCamera(fov_ang, cam_aspect, near_z, far_z * 2);

	const float fov_tan = tanf(fov_ang / 180.0f * 3.14159265f * 0.5f);

	const float near_x = fov_tan * near_z;
	const float near_y = cam_aspect * fov_tan * near_z;
	const float far_x = fov_tan * far_z;
	const float far_y = cam_aspect * fov_tan * far_z;

	const Vec3 near_top_left (-near_x, -near_y, near_z);
	const Vec3 near_top_right( near_x, -near_y, near_z);
	const Vec3 near_bot_left (-near_x,  near_y, near_z);
	const Vec3 near_bot_right( near_x,  near_y, near_z);

	const Vec3 far_top_left (-far_x, -far_y, far_z);
	const Vec3 far_top_right( far_x, -far_y, far_z);
	const Vec3 far_bot_left (-far_x,  far_y, far_z);
	const Vec3 far_bot_right( far_x,  far_y, far_z);

	MeshData frustumMeshData;
	frustumMeshData.SetAttrAvailability(ATTR_POS_MASK);
	
#define expand_vec(vec) vec.x(), vec.y(), vec.z()

	frustumMeshData.SetAttr<ATTR_POS_INDEX>(std::vector<float>{
		expand_vec(near_top_left),
		expand_vec(near_top_right),
		expand_vec(near_bot_left),
		expand_vec(near_bot_right),
		expand_vec(far_top_left),
		expand_vec(far_top_right),
		expand_vec(far_bot_left),
		expand_vec(far_bot_right)
	});
#undef expand_vec

	constexpr uint32_t ntl = 0;
	constexpr uint32_t ntr = 1;
	constexpr uint32_t nbl = 2;
	constexpr uint32_t nbr = 3;
	constexpr uint32_t ftl = 4;
	constexpr uint32_t ftr = 5;
	constexpr uint32_t fbl = 6;
	constexpr uint32_t fbr = 7;

	frustumMeshData.SetIndices(std::vector<uint32_t>{
		ntl, ntr, nbr, nbr, nbl, ntl,//near
		ftl, ftr, fbr, fbr, fbl, ftl,//far
		ntl, ftl, fbl, fbl, nbl, ntl,//left
		ntr, ftr, fbr, fbr, nbr, ntr,//right
		ntl, ntr, ftr, ftr, ftl, ntl,//top
		nbl, nbr, fbr, fbr, fbl, nbl//bot
	});

	back->RegisterMesh("frustum mesh", Mesh(std::move(frustumMeshData)));
	back->LoadMesh("C:\\Users\\zohich\\Desktop\\code_and_learn_stuff\\cpu_render_repo\\cpu_render\\app\\meshes\\katana.glb", 
		"katana mesh");

	const RenderShape& frustumShape = back->MakeRenderShape("frustum shape", "frustum mesh", "simplest material");
	const RenderShape& katanaShape = back->MakeRenderShape("katana shape", "katana mesh", "simplest material");
	frustmObj = new RenderInstance(&frustumShape);
	katanaObj = new RenderInstance(&katanaShape);
	back->AddInstance(frustmObj);
	//back->AddInstance(katanaObj);
	katanaObj->transform.pos = Vec3(0, 0, 25.5f);
	frustmObj->transform.pos = Vec3(0, 0, 0.12f);
}

void update(RenderBackend* back) {
	
	if (olcBack->GetKey(olc::W).bHeld) {
		frustmObj->transform.pos.z() += 0.124f * back->DeltaTime();
	}
	if (olcBack->GetKey(olc::S).bHeld) {
		frustmObj->transform.pos.z() -= 0.124f * back->DeltaTime();
	}
	elapsedTime += back->DeltaTime();
	//frustmObj->transform.pos.z() -= 0.124f*back->DeltaTime();
	frustmObj->transform.rot.x() = 0.724f*sinf(elapsedTime * 2.5f);

	

	katanaObj->transform.rot.x() += 1.5f * back->DeltaTime();
	//katanaObj->transform.pos.x() = sinf(elapsedTime) * 45.0f;
	//katanaObj->transform.pos.z() = 16.0f + sinf(elapsedTime * 3.5f) * 25.0f;
	back->PrintText(20, 20, std::to_string(frustmObj->transform.pos.z()));
	
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
	olcBack = &game;
	game.Run(400, 400, false);

	return 0;
}


