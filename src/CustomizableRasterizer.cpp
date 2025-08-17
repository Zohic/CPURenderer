#include "CustomizableRasterizer.h"

namespace cpuRenderSimple {
    CustomizableRasterizer::CustomizableRasterizer(VertexBufferBase* ivertBuffer, IDrawingFunctional* idrawingFunctional) :
        RasterizerBase(ivertBuffer, idrawingFunctional) {
    }

    void CustomizableRasterizer::ClipBuffer() {
        vertexBuffer->ResetIterators(false);

        for (auto& t : renderTrigs_)
            t.Free();

        renderTrigs_.clear();
        renderTrigs_.reserve(vertexBuffer->GetVertexCount() / 3);

        DEBUGPRINT("about to clip, trigs: %zu\n", vertexBuffer->GetVertexCount() / 3);

        size_t verts[2] = { 0, 0 };

        while (!vertexBuffer->End()) {
            verts[0]++;

            DEBUGPRINT("\t porcess: verts clipped: %i/%i\n", verts[0], verts[1]);

            const VertexData& v1 = vertexBuffer->GetVert();
            const VertexData& v2 = vertexBuffer->GetVert();
            const VertexData& v3 = vertexBuffer->GetVert();

            //renderTrigs_.push_back(triag);
            //continue;

            const Vec4& p1 = v1.GetPos();
            const Vec4& p2 = v2.GetPos();
            const Vec4& p3 = v3.GetPos();

            //renderTrigs_.push_back(TriangleWrap(&v1));
            //continue;

            if (p1.x() > p1.w() && p2.x() > p2.w() && p3.x() > p3.w())
                continue;
            if (p1.x() < -p1.w() && p2.x() < -p2.w() && p3.x() < -p3.w())
                continue;
            if (p1.y() > p1.w() && p2.y() > p2.w() && p3.y() > p3.w())
                continue;
            if (p1.y() < -p1.w() && p2.y() < -p2.w() && p3.y() < -p3.w())
                continue;
            //if (p1.z() < -p1.w() && p2.z() < -p2.w() && p3.z() < -p3.w())
            //    continue;
            //if (p1.y() < 0 && p2.y() < 0 && p3.y() < 0)
            //    continue;
            if (p1.z() < 0 && p2.z() < 0 && p3.z() < 0)
                continue;
            
            //renderTrigs_.push_back(TriangleWrap(&v1));
            //continue;

            //printf("culled triganle : %zu/%zu\n", verts[0], verts[1]);

            const Vec4 planes[5]{
                Vec4(0.0f, 0.0f, -1.0f, 0.5f),//near
                Vec4(1.0f, 0.0f, 0.0f, 0.5f),//left
                Vec4(-1.0f, 0.0f, 0.0f, 0.5f),//rigth
                Vec4(0.0f, 1.0f, 0.0f, 0.5f),//bottom
                Vec4(0.0f, -1.0f, 0.0f, 0.5f),//top
            };


            uint8_t newTrigs = 0;
            std::vector<TriangleWrap> to_check;
            //to_check.reserve(5);
            to_check.emplace_back(&v1);


            for (uint32_t p = 0; p < 5; ++p) {
                uint8_t was_size = to_check.size();
                for (uint32_t t = 0; t < was_size; t++) {
                    int n = cpuRenderBase::utils::ClipAgainstPlane(planes[p], t, to_check, vertexBuffer->GetMaterial().GetAttrList());
                }
            }

            //if (to_check.size() > 1) {
                for (auto& t : to_check) {
                    t[0].SetColor(Vec4(1.0f, 0.0f, 0.0f, 1.0f), vertexBuffer->GetMaterial().GetAttrList());
                    t[1].SetColor(Vec4(1.0f, 0.0f, 0.0f, 1.0f), vertexBuffer->GetMaterial().GetAttrList());
                    t[2].SetColor(Vec4(1.0f, 0.0f, 0.0f, 1.0f), vertexBuffer->GetMaterial().GetAttrList());
                }
            //}

            DEBUGPRINT("\t porcess: new trigs %zu\n", to_check.size());

            for (auto& t : to_check) {
                renderTrigs_.push_back(std::move(t));
            }

            verts[1]++;
        }

        DEBUGPRINT("verts clipped: %i/%i", verts[0], verts[1]);
    }

    void CustomizableRasterizer::DrawWires() {

        ClipBuffer();

        DEBUGPRINT("cleared buffer before drawing wires\n");
        int tind = 0;

        std::vector<Vec4> drawn;
        std::cout << renderTrigs_.size() << '\n';

        for (const auto& trig : renderTrigs_) {
            const VertexData& v1 = trig[0];
            const VertexData& v2 = trig[1];
            const VertexData& v3 = trig[2];


            
            //if (triToDraw != tind) {
            //    tind++;
            //    continue;
            //}


            DEBUGPRINT("about to draw such triangle (%i):\n", tind);
            tind++;
            //v1.PrintPos();
            //v2.PrintPos();
            //v3.PrintPos();

            const Material* const mat = &(vertexBuffer->GetMaterial());
            const FragmentShader& shader = mat->GetFragmentShader();

            Vec4 p1 = v1.GetPos();
            Vec4 p2 = v2.GetPos();
            Vec4 p3 = v3.GetPos();


            // Perspective devision
            p1.x() /= p1.w();
            p1.y() /= p1.w();
            p2.x() /= p2.w();
            p2.y() /= p2.w();
            p3.x() /= p3.w();
            p3.y() /= p3.w();

            drawn.push_back(p1);
            drawn.push_back(p2);
            drawn.push_back(p3);

            //DEBUGPRINT("vertex shader: clip space ");
            //DEBUGPRINT("(%f, %f, %f, %f)\n", p.x(), p.y(), p.z(), p.w());

            const float szx = viewportSize.x() / 3;
            const float szy = viewportSize.y() / 3;
            const float px = viewportPos.x() + viewportSize.x() / 2;
            const float py = viewportPos.y() + viewportSize.y() / 2;

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


           

            auto DrawLineV = [&](const Vec4& _v1, const Vec4& _v2, const Vec4& clr) {
                drawingTool->DrawLine(_v1.x(), _v1.y(),
                    _v2.x(), _v2.y(), clr.x(), clr.y(), clr.z());
            };

            auto DrawHalfLine = [&](const VertexData& _v1, const VertexData& _v2) {
                Vec4 halfWay = (_v1.GetPos() + _v2.GetPos()) * 0.5f;

                drawingTool->DrawLine(_v1.GetPos().x(), _v1.GetPos().y(),
                    halfWay.x(), halfWay.y(), 255, 255, 255);
            };


            //const Vec4 c1 = (v1.GetColor(mat->GetAttrList()) + v2.GetColor(mat->GetAttrList())) * 0.5f * 256.0f;
            //const Vec4 c2 = (v2.GetColor(mat->GetAttrList()) + v3.GetColor(mat->GetAttrList())) * 0.5f * 256.0f;
            //const Vec4 c3 = (v3.GetColor(mat->GetAttrList()) + v1.GetColor(mat->GetAttrList())) * 0.5f * 256.0f;

            DrawLineV(p1, p2, Vec4(1.0f, 1.0f, 1.0f, 1.0f) * 255.0f);
            DrawLineV(p2, p3, Vec4(1.0f, 1.0f, 1.0f, 1.0f) * 255.0f);
            DrawLineV(p3, p1, Vec4(1.0f, 1.0f, 1.0f, 1.0f) * 255.0f);

        }

        /*
        vertexBuffer->ResetIterators(false);

        DEBUGPRINT("cleared buffer before drawing wires\n");
        int tind = 0;

        while (!vertexBuffer->End()) {
            const VertexData& v1 = vertexBuffer->GetVert();
            const VertexData& v2 = vertexBuffer->GetVert();
            const VertexData& v3 = vertexBuffer->GetVert();



            //if (triToDraw != tind) {
            //    tind++;
            //    continue;
            //}


            DEBUGPRINT("about to draw such triangle (%i):\n", tind);
            tind++;
            //v1.PrintPos();
            //v2.PrintPos();
            //v3.PrintPos();

            const Material* const mat = &(vertexBuffer->GetMaterial());
            const FragmentShader& shader = mat->GetFragmentShader();

            Vec4 p1 = v1.GetPos();
            Vec4 p2 = v2.GetPos();
            Vec4 p3 = v3.GetPos();


#pragma region Perspective Division
                p1.x() /= p1.w();
                p1.y() /= p1.w();
                p2.x() /= p2.w();
                p2.y() /= p2.w();
                p3.x() /= p3.w();
                p3.y() /= p3.w();
#pragma endregion


                //DEBUGPRINT("vertex shader: clip space ");
                //DEBUGPRINT("(%f, %f, %f, %f)\n", p.x(), p.y(), p.z(), p.w());

                const float szx = viewportSize.x() / 2;
                const float szy = viewportSize.y() / 2;
                const float px = szx + viewportPos.x();
                const float py = szy + viewportPos.y();

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




                DrawLineV(p1, p2);

                DrawLineV(p2, p3);

                DrawLineV(p3, p1);

            }

            */
    }

    void CustomizableRasterizer::RenderTriangle(const VertexData& p1, const VertexData& p2, const VertexData& p3) {
        /*const Vec3& p1p = p1.GetPos();
        const Vec3& p2p = p1.GetPos();
        const Vec3& p3p = p1.GetPos();

        drawingTool->FillTriangle(p1p.x(), p1p.y(), p2p.x(), p2p.y(), p3p.x(), p3p.y());*/
    }

    void CustomizableRasterizer::RenderTriangles() {
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
}