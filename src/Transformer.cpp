/*#include "Transformer.h"

namespace cpuRenderSimple {

    template<typename VertexBufferType>
    Transformer<VertexBufferType>::Transformer(VertexBufferType* buff): TransformerBase<VertexBufferType>(buff) {

    }

    template<typename VertexBufferType>
    void Transformer<VertexBufferType>::SetCulling(CullType c) {
        culling = c;
    }

    template<typename VertexBufferType>
    void Transformer<VertexBufferType>::SetCamera(float fovY, float aspectRatio, float front, float back) {
        DEBUGPRINT("SETTING CAMERA\n");
        this->cam.SetFrustum(fovY, aspectRatio, front, back);
    }

    template<typename VertexBufferType>
    void Transformer<VertexBufferType>::ProcessRenderInstance(const RenderInstance& renderInst) {

        const RenderShape& shape = *(renderInst.GetShape());
        const Mesh& mesh = renderInst.GetShape()->GetMesh();
        const Material& mat = renderInst.GetShape()->GetMaterial();
        const VertexShader& vertShader = (VertexShader)renderInst.GetShape()->GetMaterial().GetVertexShader();

        VertexData vData[3];
        VertexShaderInput vInput;


        vInput.translationMatrix = renderInst.transform.GetTranslationMatrix();
        vInput.rotationMatrix    = renderInst.transform.GetRotationMatrix();
        vInput.scalingMatrix     = renderInst.transform.GetScaleMatrix();
        vInput.worldMatrix       = renderInst.transform.GetWorldMatrix();

        vInput.projectionMatrix  = this->cam.GetProjectionMatrix();
        vInput.availableAttrs    = mat.GetAttrList();

        volatile int num = 0;

        int i = 0;
        shape.ResetIterator();
        while (!shape.EndOfTriangles()) {
            DEBUGPRINT("working on triangle: %i\n", i);
            Vec3 points[3];
            i++;

            shape.GetTrinagle(vData);

            vertShader(vData[0], vInput);
            vertShader(vData[1], vInput);
            vertShader(vData[2], vInput);

            DEBUGPRINT("passed vertices through shader\n");

            //these points are expected to be in clip space
            points[0] = *((Vec3*)&(vData[0].GetPos()));
            points[1] = *((Vec3*)&(vData[1].GetPos()));
            points[2] = *((Vec3*)&(vData[2].GetPos()));


            Vec3 croosProd;
            gmtl::cross(croosProd, Vec3(points[1] - points[0]), Vec3(points[2] - points[0]));

            bool toDraw = true;

            
            switch (culling)
            {
            case CullType::NO:
                break;
            case CullType::FRONT:
                if (gmtl::dot(croosProd, points[0]) > 0.001f)
                    toDraw = false;
                break;
            case CullType::BACK:
                if (gmtl::dot(croosProd, points[0]) < -0.001f)
                    toDraw = false;
                break;
            }
            


            if (!toDraw) {
                DEBUGPRINT("triangle is culled\n");
                continue;
            }


            num++;

            this->buffer->InsertVertex(std::move(vData[0]), &mat);
            this->buffer->InsertVertex(std::move(vData[1]), &mat);
            this->buffer->InsertVertex(std::move(vData[2]), &mat);

            DEBUGPRINT("pushed verties into vertex buffer\n");
        }

        DEBUGPRINT("total number of not culled triangles: %i\n", num);
        DEBUGPRINT("done transformation!\n");
    }

}*/