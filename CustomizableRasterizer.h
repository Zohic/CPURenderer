#pragma once
#include "Render.h"
#include "ResourceStorage.h"
#include "VertexBuffer.h"

int vertToDraw = 0;
int triToDraw = 0;

namespace cpuRenderSimple {
    using namespace cpuRenderBase;

    template<typename VertexBufferType=VertexBufferBase, typename DrawingFunctional=IDrawingFunctional>
    class CustomizableRasterizer final : public RasterizerBase<VertexBufferType, DrawingFunctional> {
    public:
        enum class CullType {
            FRONT, BACK, BOTH
        };

    private:
        CustomizableRasterizer() = delete;
        CullType culling = CullType::FRONT;
    public:
        
//both options are working, you could write this->vertexBuffer 
#define vertBuf CustomizableRasterizer::vertexBuffer
#define drawingTool CustomizableRasterizer::drawingTool
#define vpPos CustomizableRasterizer::viewportPos
#define vpSize CustomizableRasterizer::viewportSize

        CustomizableRasterizer(VertexBufferType* ivertBuffer, DrawingFunctional* idrawingFunctional) :
            RasterizerBase<VertexBufferType, DrawingFunctional>(ivertBuffer, idrawingFunctional) {
        }

        
        void DrawWires() override {
            vertBuf->ResetIterators(false);

            DEBUGPRINT("cleared buffer before drawing wires\n");
            int tind = 0;
            while (!vertBuf->End()) {
                const VertexData& v1 = vertBuf->GetVert();
                const VertexData& v2 = vertBuf->GetVert();
                const VertexData& v3 = vertBuf->GetVert();

                //if (triToDraw != tind) {
                //    tind++;
                //    continue;
                //}
                    

                DEBUGPRINT("about to draw such triangle (%i):\n", tind);
                tind++;
                v1.PrintPos();
                v2.PrintPos();
                v3.PrintPos();

                const Material* mat = &(vertBuf->GetMaterial());
                DEBUGPRINT("all set: %i\n", mat);
                const FragmentShader& shader = mat->GetFragmentShader();

                Vec4 p1 = v1.GetPos();
                Vec4 p2 = v2.GetPos();
                Vec4 p3 = v3.GetPos();

                p1.x() /= p1.w();
                p1.y() /= p1.w();
                p2.x() /= p2.w();
                p2.y() /= p2.w();
                p3.x() /= p3.w();
                p3.y() /= p3.w();

                //DEBUGPRINT("vertex shader: clip space ");
                //DEBUGPRINT("(%f, %f, %f, %f)\n", p.x(), p.y(), p.z(), p.w());

                const float szx = vpSize.x() / 2;
                const float szy = vpSize.y() / 2;
                const float px = szx + vpPos.x();
                const float py = szy + vpPos.y();

                p1.x() *= szx;
                p1.y() *= szy;
                p2.x() *= szx;
                p2.y() *= szy;
                p3.x() *= szx;
                p3.y() *= szy;

                p1.x() += px;
                p1.y() += py;
                p2.x() += px;
                p2.y() += py;
                p3.x() += px;
                p3.y() += py;
                
                auto DrawLineV = [&](const Vec4& _v1, const Vec4& _v2) {
                    drawingTool->DrawLine(_v1.x(), _v1.y(),
                        _v2.x(), _v2.y(), 255, 255, 255);
                };

                auto DrawHalfLine = [&](const VertexData& _v1, const VertexData& _v2) {
                    Vec4 halfWay = (_v1.GetPos() + _v2.GetPos()) * 0.5f;

                    drawingTool->DrawLine(_v1.GetPos().x(), _v1.GetPos().y(),
                        halfWay.x(), halfWay.y(), 255, 255, 255);
                };
                


                //if (vertToDraw == 0)
                //    DrawHalfLine(v1, v2);
                //if (vertToDraw == 1)
                DrawLineV(p1, p2);
                //if (vertToDraw == 2)
                //    DrawHalfLine(v2, v1);

                //if (vertToDraw == 3)
                //    DrawHalfLine(v2, v3);
                //if (vertToDraw == 4)
                DrawLineV(p2, p3);
                //if (vertToDraw == 5)
                //    DrawHalfLine(v3, v2);

                //if (vertToDraw == 6)
                //    DrawHalfLine(v3, v1);
                //if (vertToDraw == 7)
                DrawLineV(p3, p1);
                //if (vertToDraw == 8)
                //    DrawHalfLine(v1, v3);
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
