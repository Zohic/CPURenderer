#pragma once
#include "Render.h"

namespace cpuRenderSimple {
    using namespace cpuRenderBase;

    template<typename VertexBufferType>
    class Transformer final: public TransformerBase<VertexBufferType> {
    public:
        enum class CullType {
            FRONT, BACK, NO
        };

    private:
        CullType culling = CullType::FRONT;

    public:
        
        Transformer(VertexBufferType* buff):TransformerBase<VertexBufferType>(buff) {

        }

        void SetCulling(CullType c) {
            culling = c;
        }

        void SetCamera(float fovY, float aspectRatio, float front, float back) {
            DEBUGPRINT("SETTING CAMERA\n");
            this->cam.SetFrustum(fovY, aspectRatio, front, back);
        }

        void ProcessRenderInstance(const RenderInstance& renderInst) override {
                const MeshData& mesh = renderInst.GetShape()->GetMesh();
                const Material& mat = renderInst.GetShape()->GetMaterial();
                const VertexShader& vertShader = (VertexShader)renderInst.GetShape()->GetMaterial().GetVertexShader();

                VertexData vData[3];
                VertexShaderInput vInput;
              

                vInput.translationMatrix = renderInst.transform.GetTranslationMatrix();
                vInput.rotationMatrix = renderInst.transform.GetRotationMatrix();
                vInput.scalingMatrix = renderInst.transform.GetScaleMatrix();

                //TODO: undertand what the fuck
                /*for (int i = 0; i < 5; i++) {
                    printf("vinp(%i): %u\t", i, vInput.availableAttrs[i]);
                    printf("attrs(%i): %u\n", i, attrReqList[i]);
                    vInput.availableAttrs[i] = attrReqList[i];
                    //memcpy((&(vInput.availableAttrs))+i, (&attrReqList) + i, 1);
                }*/
                //for some reason this doesnt work
                //printf("HAH3: %u\n", (size_t)(const void*)(&attrReqList) - (size_t)(void*)(&(vInput.availableAttrs)));
                //memcpy((void*)(&(vInput.availableAttrs)), (void*)(&attrReqList), 5ui64 * sizeof(bool));


                vInput.projectionMatrix = this->cam.GetProjectionMatrix();
                //PrintMatrix(vInput.projectionMatrix);
                vInput.availableAttrs = mat.GetAttrList();

                int num = 0;
                
                
                for (int i = 0; i < mesh.GetTriangleCount(); i++) {
                    DEBUGPRINT("working on triangle: %i\n", i);
                    Vec3 points[3];

                    vData[0].Init(mat.GetAttrList());
                    vData[0].SetFromMesh(&mesh, mat.GetAttrList(), i * 3 + 0);
                    vertShader(vData[0], vInput);

                    vData[1].Init(mat.GetAttrList());
                    vData[1].SetFromMesh(&mesh, mat.GetAttrList(), i * 3 + 1);
                    vertShader(vData[1], vInput);

                    vData[2].Init(mat.GetAttrList());
                    vData[2].SetFromMesh(&mesh, mat.GetAttrList(), i * 3 + 2);
                    vertShader(vData[2], vInput);

                    //these points are expected to be in clip space

                    points[0] = *((Vec3*)&(vData[0].GetPos()));
                    points[1] = *((Vec3*)&(vData[1].GetPos()));
                    points[2] = *((Vec3*)&(vData[2].GetPos()));
                   

                    Vec3 croosProd;
                    gmtl::cross(croosProd, Vec3(points[1] - points[0]), Vec3(points[2] - points[0]));

                    bool toDraw = true;

                    if (culling != CullType::NO) {
                        switch (culling)
                        {
                        case CullType::FRONT:
                            if (gmtl::dot(croosProd, points[0]) > 0.001f)
                                toDraw = false;
                            break;
                        case CullType::BACK:
                            if (gmtl::dot(croosProd, points[0]) < -0.001f)
                                toDraw = false;
                            break;
                        }
                    }


                    if (!toDraw)
                        continue;
                   
                    num++;

                    this->buffer->InsertVertex(vData[0], &mat);
                    DEBUGPRINT("\tfirst vert\n");

                    this->buffer->InsertVertex(vData[1], &mat);
                    DEBUGPRINT("\second vert\n");

                    this->buffer->InsertVertex(vData[2], &mat);
                    DEBUGPRINT("\tthird vert\n");
                }
                DEBUGPRINT("total number of tringles: %i\n", num);
                DEBUGPRINT("done transformation!\n");
        }
    };

}