#pragma once
#include "Render.h"

namespace cpuRenderSimple {
    using namespace cpuRenderBase;

    template<typename VertexBufferType>
    class Transformer final: public TransformerBase<VertexBufferType> {
        //std::unordered_map<Material*, > buffers;
    public:

        Transformer(VertexBufferType* buff):TransformerBase<VertexBufferType>(buff) {

        }

        void ProcessRenderInstance(const RenderInstance& renderInst) override {
            
                const MeshData& mesh = renderInst.GetShape()->GetMesh();
                const Material& mat = renderInst.GetShape()->GetMaterial();
                const VertexShader& vertShader = (VertexShader)renderInst.GetShape()->GetMaterial().GetVertexShader();

                VertexData vData;
                bool attrReqList[5];
                memcpy(attrReqList, mat.GetAttrList(), 5ui64 * sizeof(bool));

                for (int i = 0; i < mesh.GetTriangleCount(); i++) {
                    printf("working on triangle: %i\n", i);
                    vData.FillFromMesh(&mesh, attrReqList, i * 3);
                    this->buffer->InsertVertex(vertShader(vData), &mat);

                    vData.FillFromMesh(&mesh, attrReqList, i * 3 + 1);
                    this->buffer->InsertVertex(vertShader(vData), &mat);

                    vData.FillFromMesh(&mesh, attrReqList, i * 3 + 2);
                    this->buffer->InsertVertex(vertShader(vData), &mat);
                }
                printf("done trans!\n");
        }
    };

}