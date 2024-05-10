#pragma once
#include "Render.h"
#include "ResourceStorage.h"
#include "VertexBuffer.h"

namespace cpuRenderSimple {
    using namespace cpuRenderBase;

    template<typename VertexBufferType=VertexBufferBase, typename DrawingFunctional=IDrawingFunctional>
    class CustomizableRasterizer final : public RasterizerBase<VertexBufferType, DrawingFunctional> {
        CustomizableRasterizer() = delete;
    public:

//both options are working, you could write this->vertexBuffer 
#define vertBuf CustomizableRasterizer::vertexBuffer
#define drawingTool CustomizableRasterizer::drawingTool

        CustomizableRasterizer(VertexBufferType* ivertBuffer, DrawingFunctional* idrawingFunctional) :
            RasterizerBase<VertexBufferType, DrawingFunctional>(ivertBuffer, idrawingFunctional) {
        }

        void DrawWires() override {
            vertBuf->Clear();

            printf("cleared buffer before drawing wires\n");
            while (!vertBuf->End()) {
                const VertexData& v1 = vertBuf->GetVert();
                const VertexData& v2 = vertBuf->GetVert();
                const VertexData& v3 = vertBuf->GetVert();

                v1.Print();
                v2.Print();
                v3.Print();

                const Material* mat = &(vertBuf->GetMaterial());
                printf("all set: %i\n", mat);
                const FragmentShader& shader = mat->GetFragmentShader();

                

                drawingTool->DrawLine(v1.GetPos().x() * 100, v1.GetPos().y() * 100,
                    v2.GetPos().x() * 100, v2.GetPos().y() * 100, 255, 255, 255);
                printf("drawn first\n");
                drawingTool->DrawLine(v1.GetPos().x() * 100, v1.GetPos().y() * 100,
                    v2.GetPos().x() * 100, v2.GetPos().y() * 100, 255, 255, 255);

                drawingTool->DrawLine(v1.GetPos().x() * 100, v1.GetPos().y() * 100,
                    v2.GetPos().x() * 100, v2.GetPos().y() * 100, 255, 255, 255);

            }

            
        }

        void RenderTriangle(const VertexData& p1, const VertexData& p2, const VertexData& p3) {
            /*const Vec3& p1p = p1.GetPos();
            const Vec3& p2p = p1.GetPos();
            const Vec3& p3p = p1.GetPos();

            drawingTool->FillTriangle(p1p.x(), p1p.y(), p2p.x(), p2p.y(), p3p.x(), p3p.y());*/
        }

        void RenderTriangles() override {
            /*vertBuf->Reset();

            VertexData p1, p2, p3;
            Material* mat;
            vertBuf->
            while(!vertBuf->endOfBuffer()) {
                p1 = vertBuf->GetNext();
                p2 = vertBuf->GetNext();
                p3 = vertBuf->GetNext();
                RenderTriangle(p1, p2, p3);
            }*/

        }
#undef vertBuf
#undef drawingTool

    };
}
