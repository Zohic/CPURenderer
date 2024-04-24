#pragma once
#include "Render.h"

namespace cpuRenderSimple {
    using namespace cpuRenderBase;

    template<typename VertexBufferType, typename DrawingFunctional>
    class CustomizableRasterizer final : public RasterizerBase<VertexBufferType, typename DrawingFunctional> {
        CustomizableRasterizer() = delete;
    public:
        CustomizableRasterizer(VertexBufferType* ivertBuffer, DrawingFunctional* idrawingFunctional) : 
            RasterizerBase<VertexBufferType, DrawingFunctional>(ivertBuffer, idrawingFunctional) {
            
        }

        void DrawWires() override {
            //this macro neede only because Visual Studio can't see vertexBuffer without "this->", probably because of all the templates
#define vertBuf this->vertexBuffer
#define drawingTool this->drawingTool

            std::cout << "drawing " << vertBuf->GetVertexCount() << " vertices\n";

            for (int v = 0; v < vertBuf->GetVertexCount(); v+=3) {


                std::cout << "\tvert 1: (" << vertBuf->GetVertX(v + 0) << "; "<< vertBuf->GetVertY(v + 0) << ")\n";
                std::cout << "\tvert 2: (" << vertBuf->GetVertX(v + 1) << "; " << vertBuf->GetVertY(v + 1) << ")\n";
                std::cout << "\tvert 3: (" << vertBuf->GetVertX(v + 2) << "; " << vertBuf->GetVertY(v + 2) << ")\n";
                

                drawingTool->DrawLine(vertBuf->GetVertX(v), vertBuf->GetVertY(v),
                    vertBuf->GetVertX(v + 1), vertBuf->GetVertY(v + 1));

                drawingTool->DrawLine(vertBuf->GetVertX(v + 1), vertBuf->GetVertY(v + 1),
                    vertBuf->GetVertX(v + 2), vertBuf->GetVertY(v + 2));

                drawingTool->DrawLine(vertBuf->GetVertX(v), vertBuf->GetVertY(v),
                    vertBuf->GetVertX(v + 2), vertBuf->GetVertY(v + 2));
            }
        }
    };
}
