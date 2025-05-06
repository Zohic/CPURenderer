#pragma once
#include "Render.h"
#include "ResourceStorage.h"
#include "VertexBuffer.h"

namespace cpuRenderSimple {
    using namespace cpuRenderBase;

    class CustomizableRasterizer final : public RasterizerBase {
    public:
        enum class CullType {
            FRONT, BACK, BOTH
        };

    private:
        CustomizableRasterizer() = delete;
        CullType culling = CullType::BOTH;
        std::vector<TriangleWrap> renderTrigs_;
    public:
        
        CustomizableRasterizer(VertexBufferBase* ivertBuffer, IDrawingFunctional* idrawingFunctional);

        //lineStart will be exchabged with interpolated value\
        //note that lineStart will be interpolated to the plane even if its on the right side of it

        void ClipBuffer();


        void DrawWires() override;
        void RenderTriangle(const VertexData& p1, const VertexData& p2, const VertexData& p3);
        void RenderTriangles() override;
    };
}
