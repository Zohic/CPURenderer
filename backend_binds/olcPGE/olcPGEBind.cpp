#define OLC_PGE_APPLICATION

#include "olcPGEBind.h"


olcPGEBind::olcPGEBind(RenderBackend::BackendFunction setupFunc, RenderBackend::BackendFunction updateFunc):
	cpuRenderBase::RenderBackend(setupFunc, updateFunc) {
	if (setupFunc == nullptr || updateFunc == nullptr)
		throw std::logic_error("trying to make OLC PGE Backend with nullptr");
}


void olcPGEBind::DrawPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) const {

}

void olcPGEBind::DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint8_t r, uint8_t g, uint8_t b) const {
	olc::PixelGameEngine::DrawLine(x1, y1, x2, y2, olc::Pixel(r, g, b));
}

void olcPGEBind::FillTriangle(
	uint32_t x1, uint32_t y1, 
	uint32_t x2, uint32_t y2, 
	uint32_t x3, uint32_t y3, 
	uint8_t r, uint8_t g, uint8_t b) const {
	olc::PixelGameEngine::FillTriangle(x1, y1, x2, y2, x3, y3, olc::Pixel(r, g, b));
}

bool olcPGEBind::OnUserCreate() {
	SetupCamera(100.0f, 1.0f, 0.01f, 1000.0f);
	SetupViewport(0, 0, ScreenWidth(), ScreenHeight());

	this->setupFunction(this);

	return true;
}

bool olcPGEBind::OnUserUpdate(float dt) {
	olc::PixelGameEngine::Clear(olc::BLACK);
	this->deltaTime = dt;
	this->updateFunction(this);
	this->PipeLineProcess();

	return true;
}

void olcPGEBind::Run(uint32_t width, uint32_t height, bool fullscreen) {
	if (olc::PixelGameEngine::Construct(width, height, 1, 1, fullscreen, false) == olc::rcode::OK) {
		olc::PixelGameEngine::Start();
		return;
	}
	
	throw std::logic_error("couldn't construct olc::pixel game engine");
}
