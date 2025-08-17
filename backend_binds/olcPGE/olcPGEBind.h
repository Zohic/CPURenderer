#pragma once

#include "../RenderBackend.h"
#include <olcPixelGameEngine.h>


class olcPGEBind final : public cpuRenderBase::RenderBackend, public olc::PixelGameEngine {

private:
	olcPGEBind()                                   = delete;
	olcPGEBind(const olcPGEBind& other)            = delete;
	olcPGEBind(olcPGEBind&& old)                   = delete;
	olcPGEBind& operator=(const olcPGEBind& other) = delete;
	olcPGEBind& operator=(olcPGEBind&& old)        = delete;

public:
	olcPGEBind(RenderBackend::BackendFunction setupFunc, RenderBackend::BackendFunction updateFunc);

	// implement IDrawingFunctional
	void DrawPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) const override;
	void DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint8_t r, uint8_t g, uint8_t b) const override;
	void FillTriangle(
		uint32_t x1, uint32_t y1, 
		uint32_t x2, uint32_t y2, 
		uint32_t x3, uint32_t y3, 
		uint8_t r, uint8_t g, uint8_t b) const override;

	void PrintText(uint32_t, uint32_t, const std::string&) const override;

	// implement RenderBackend
	void Run(uint32_t width, uint32_t height, bool fullscreen) override;

	// implement PixelGameEngine
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
	

};
