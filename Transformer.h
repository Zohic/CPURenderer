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

        void SetCamera(float fovY, float aspectRatio, float front, float back) {
            DEBUGPRINT("SETTING CAMERA\n");
            this->cam.SetFrustum(fovY, aspectRatio, front, back);
        }

        void ProcessRenderInstance(const RenderInstance& renderInst) override {
            
                const MeshData& mesh = renderInst.GetShape()->GetMesh();
                const Material& mat = renderInst.GetShape()->GetMaterial();
                const VertexShader& vertShader = (VertexShader)renderInst.GetShape()->GetMaterial().GetVertexShader();

                VertexData vData;
                VertexShaderInput vInput;
                
                
                bool attrReqList[5];
                memcpy(attrReqList, mat.GetAttrList(), 5ui64 * sizeof(bool));
                

                vInput.translationMatrix = renderInst.transform.GetTranslationMatrix();
                vInput.rotationMatrix = renderInst.transform.GetRotationMatrix();
                vInput.scalingMatrix = renderInst.transform.GetScaleMatrix();

                memcpy(&(vInput.availableAttrs), mat.GetAttrList(), 5ui64 * sizeof(bool));

                vInput.projectionMatrix = this->cam.GetProjectionMatrix();

                PrintMatrix(vInput.projectionMatrix);

                vData.SetSize(attrReqList);

                for (int i = 0; i < mesh.GetTriangleCount(); i++) {
                    DEBUGPRINT("working on triangle: %i\n", i);

                    vData.SetFromMesh(&mesh, attrReqList, i * 3);
                   // DEBUGPRINT("SET FROM MESH IN TRANSFOMER: ");
                    vData.Print();
                    vertShader(vData, vInput);
                    this->buffer->InsertVertex(vData, &mat);

                    vData.SetFromMesh(&mesh, attrReqList, i * 3 + 1);
                    vertShader(vData, vInput);
                    this->buffer->InsertVertex(vData, &mat);

                    vData.SetFromMesh(&mesh, attrReqList, i * 3 + 2);
                    vertShader(vData, vInput);
                    this->buffer->InsertVertex(vData, &mat);
                }
                DEBUGPRINT("done transformation!\n");
        }
    };

}