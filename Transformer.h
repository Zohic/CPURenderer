#pragma once
#include "Render.h"

namespace cpuRenderSimple {
    using namespace cpuRenderBase;

    template<typename VertexBufferType>
    class Transformer final: public TransformerBase<VertexBufferType> {
        std::unordered_map<size_t, std::vector<float>> buffers;
    public:

        Transformer(IResourceStorge* stor, VertexBufferType* buff):TransformerBase<VertexBufferType>(stor, buff) {

        }

        void ProcessRenderObject(const RenderInstance& renderInst) override {
                
            const MeshData& mesh = renderInst.GetShape()->GetMesh();

            const VertexShader vertShader = (VertexShader)renderInst.GetShape()->GetMaterial().VertexShader();

            for (int i = 0; i < mesh.GetVertexCount(); i++) {

                VertexShaderInput input{ mesh.GetAttr(MeshData::VERTEX_OFFSET, i) };
                this->buffer->InsertVertex(vertShader(input));
            }
        }

        void FillBuffer() {

        }

    };

}