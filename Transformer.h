#pragma once
#include "Render.h"

namespace cpuRenderSimple {
    using namespace cpuRenderBase;

    class Transformer final: public TransformerBase {
        std::unordered_map<size_t, std::vector<float>> buffers;
    public:
        void ProcessRenderObject(const RenderInstance& renderInst) override {
                
            const MeshData& mesh = renderInst.GetShape()->GetMesh();
            for (int i = 0; i < mesh.GetVertexCount(); i++) {
                Vec3 vert(
                    mesh.GetAttr(MeshData::VERTEX_OFFSET, i),
                    mesh.GetAttr(MeshData::VERTEX_OFFSET, i + 1),
                    mesh.GetAttr(MeshData::VERTEX_OFFSET, i + 2));

                buffer->InsertVertex(vert);
            }
        }

        void FillBuffer() {

        }

    };

    

}