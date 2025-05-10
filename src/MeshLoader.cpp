#define CGLTF_IMPLEMENTATION

#include <iostream>
#include "MeshLoader.h"


using namespace cpuRenderSimple;
using namespace cpuRenderBase;

using namespace std::string_literals;

void MeshLoader::OpenFile() const {

    std::cout << "loading file \n";

    //unique_ptr::reset handles deletion of previous file
    //CXX_STREAM DOESNT WORK IN REALASE MODE FOR SOME REASON, C_FILE WORKS FINE
    fileData_.reset(LoadFileData<LoadMethod::C_FILE>(fileName_, &dataSize_));

    if (!fileData_) {
        printf("failed to load file (%s) data", fileName_);
        throw std::logic_error(("failed to load file (%s) data"s + fileName_).c_str());
    }
}

void MeshLoader::ParseTree() const {

    //wasn't working until I default-initialized {} cgltf_options
    //cgltf_parse later had been returning out_of_memory result 
    //(from parse_json inside because it was trying to allocate too much json tokens)
    //but setting options.json_token_count to zero had been causing runtime failing so it must be not the only problem
    cgltf_options options{};
    options.type = cgltf_file_type::cgltf_file_type_invalid;

    //these seem to be never used
    //options.file.read = LoadFileGLTFCallback;
    //options.file.release = ReleaseFileGLTFCallback;


    cgltf_result result = cgltf_parse(&options, fileData_.get(), dataSize_, &meshData_);

    if (result != cgltf_result_success) {
        printf("could't parse gltf file: %s, error code: %i", fileName_, result);
        return;
    }


    const char* fileTypeNames[2] = { "glTF", "glb" };

    if (meshData_->file_type == cgltf_file_type_invalid)
        printf("Model format not recognized: %s\n", fileName_);
    else
        printf("Model basic data (%s) loaded successfully: %s\n", fileTypeNames[meshData_->file_type - 1], fileName_);


    printf("Meshes count: %i\n", (unsigned int)meshData_->meshes_count);
    printf("Materials count:  %i (+1 default)\n", (unsigned int)meshData_->materials_count);
    printf("Buffers count: %i\n", (unsigned int)meshData_->buffers_count);
    printf("Images count: %i\n", (unsigned int)meshData_->images_count);
    printf("Textures count: %i\n", (unsigned int)meshData_->textures_count);




    // Force reading data buffers (fills buffer_view->buffer->data)
    // NOTE: If an uri is defined to base64 data or external path, it's automatically loaded
    result = cgltf_load_buffers(&options, meshData_, fileName_);

    if (result != cgltf_result_success)
        printf("MODEL: [%s] Failed to load mesh/material buffers", fileName_);

    uint32_t primitivesCount = 0;
    for (uint32_t i = 0; i < meshData_->meshes_count; i++)
        for (uint32_t j = 0; j < meshData_->meshes[i].primitives_count; j++)
            if (meshData_->meshes[i].primitives[j].type == cgltf_primitive_type_triangles)
                primitivesCount++;

    dstMesh_->ReserveMeshCount(primitivesCount);

    printf("MODEL: Meshes: %i\n", (unsigned int)meshData_->meshes_count);
    printf("MODEL: Primitive count: %i\n", primitivesCount);
}

void MeshLoader::FillAttribute(const cgltf_accessor* const accesor, uint32_t numComp, std::vector<float>& dstVec) const {

    //reducing to smaller size does not affect capacity
    //attribute vectors are moved after each iteration, but capacity is saved manually in LoadAttributes
    dstVec.resize(accesor->count * numComp);

    size_t n = 0;
    const char* const buff = (const char*)accesor->buffer_view->buffer->data + accesor->buffer_view->offset;
    const float* const buffer = (const float*)(buff)+accesor->offset / sizeof(float);

    for (unsigned int k = 0; k < accesor->count; k++)
    {
        for (int l = 0; l < numComp; l++)
        {
            //printf("setting %i value with %i valuse\n", k* numComp + l, n + l); 
            dstVec[k * numComp + l] = buffer[n + l];
        }

        n += (size_t)(accesor->stride / sizeof(float));
    }
}

void MeshLoader::TransformVertices(const cgltf_node& node) const {
    if (node.has_matrix) {
        printf("\tobject has TRS matrix, this transformation is yet to be implemented\n");
    }
    else {
        if (node.has_scale) {
            printf("\tobject is scaled\n");
            for (size_t v = totalVerticesCount_ * 3; v < vertices_.size(); v += 3) {
                vertices_[v + 0] *= node.scale[0];
                vertices_[v + 1] *= node.scale[1];
                vertices_[v + 2] *= node.scale[2];
            }
        }


        if (node.has_rotation) {
            printf("\tobject is rotated\n");
            Quaternion quat = {
                node.rotation[0], //x 
                node.rotation[1], //y
                node.rotation[2], //z
                node.rotation[3] };//w 

            Mat4x4 rotMat;
            cpuRenderBase::utils::FillRotationMatrix(quat.toEuler(), rotMat);

            for (size_t v = totalVerticesCount_ * 3; v < vertices_.size(); v += 3) {
                Vec4 temp(vertices_[v + 0], vertices_[v + 1], vertices_[v + 2], 0);
                temp = (rotMat * temp).eval();
                vertices_[v + 0] = temp.x();
                vertices_[v + 1] = temp.y();
                vertices_[v + 2] = temp.z();
            }

        }


        if (node.has_translation) {
            printf("\tobject is translated\n");
            for (size_t v = totalVerticesCount_ * 3; v < vertices_.size(); v += 3) {
                vertices_[v + 0] += node.translation[0];
                vertices_[v + 1] += node.translation[1];
                vertices_[v + 2] += node.translation[2];
            }
        }

    }
}

void MeshLoader::LoadAttributes(MeshData& md, cgltf_mesh& mesh_ref, const cgltf_primitive& primitive, const cgltf_node& node) const {

    size_t prevCap = 0;

    for (uint32_t j = 0; j < primitive.attributes_count; j++) {

        cgltf_accessor* attribute = primitive.attributes[j].data;

        switch (primitive.attributes[j].type) {

#define CHECK_TYPES(comp_type, vec_type)\
        (attribute->component_type == comp_type) && (attribute->type == vec_type)

        case cgltf_attribute_type_position:
            // WARNING: SPECS: POSITION accessor MUST have its min and max properties defined.

            if (!CHECK_TYPES(cgltf_component_type_r_32f, cgltf_type_vec3)) {
                printf("MODEL: [%s] vertices_ attribute data format not supported, use vec3 float", fileName_);
                break;
            }

            printf("\t\tloading pos:\n");
            FillAttribute(attribute, 3, vertices_);
            TransformVertices(node);

            prevCap = vertices_.capacity();
            md.SetAttr<ATTR_POS_INDEX>(std::move(vertices_));
            vertices_.reserve(prevCap);

            attrMask_ |= ATTR_POS_MASK;


            break;

        case cgltf_attribute_type_normal:
            if (!CHECK_TYPES(cgltf_component_type_r_32f, cgltf_type_vec3)) {
                printf("MODEL: [%s] Normal attribute data format not supported, use vec3 float", fileName_);
                break;
            }
            printf("\t\tloading normal:\n");
            FillAttribute(attribute, 3, normal_);

            prevCap = normal_.capacity();
            md.SetAttr<ATTR_NORMAL_INDEX>(std::move(normal_));
            normal_.reserve(prevCap);

            attrMask_ |= ATTR_NORMAL_MASK;
            break;

        case cgltf_attribute_type_tangent:
            if (!CHECK_TYPES(cgltf_component_type_r_32f, cgltf_type_vec4)) {
                printf("MODEL: [%s] Tangent attribute data format not supported, use vec4 float", fileName_);
                break;
            }
            printf("\t\tloading tangent:\n");
            FillAttribute(attribute, 4, tangent_);

            prevCap = tangent_.capacity();
            md.SetAttr<ATTR_TANGENT_INDEX>(std::move(tangent_));
            tangent_.reserve(prevCap);

            attrMask_ |= ATTR_TANGENT_MASK;
            break;

        case cgltf_attribute_type_texcoord:
            if (!CHECK_TYPES(cgltf_component_type_r_32f, cgltf_type_vec2)) {
                printf("MODEL: [%s] Texcoords attribute data format not supported, use vec2 float", fileName_);
                break;
            }
            printf("\t\tloading texcoord:\n");
            FillAttribute(attribute, 2, texcoord_);

            prevCap = texcoord_.capacity();
            md.SetAttr<ATTR_TEXCOORD_INDEX>(std::move(texcoord_));
            texcoord_.reserve(prevCap);

            attrMask_ |= ATTR_TEXCOORD_MASK;
            break;

        case cgltf_attribute_type_color:

            // WARNING: SPECS: All components of each COLOR_n accessor element MUST be clamped to [0.0, 1.0] range.
            if (CHECK_TYPES(cgltf_component_type_r_32f, cgltf_type_vec4)) {
                printf("\t\tloading color:\n");
                FillAttribute(attribute, 4, color_);

                prevCap = color_.capacity();
                md.SetAttr<ATTR_COLOR_INDEX>(std::move(color_));
                color_.reserve(prevCap);


                attrMask_ |= ATTR_COLOR_MASK;
            }
            /*if (CHECK_TYPES(attribute, cgltf_component_type_r_8u, cgltf_type_vec4)) {

                std::vector<unsigned char> temp;
                printf("\t\tloading color:\n");
                LOAD_ATTRIBUTE(attribute, 4, unsigned char, temp);

                color.resize((int)attribute->count * 4);
                for (unsigned int c = 0; c < attribute->count * 4; c++) color[c] = (float)temp[c] / 255.0f;

            }
            else if (CHECK_TYPES(attribute, cgltf_component_type_r_16u, cgltf_type_vec4)) {

                std::vector<short> temp;
                printf("\t\tloading color:\n");
                LOAD_ATTRIBUTE(attribute, 4, unsigned short, temp);

                color.resize((int)attribute->count * 4);
                for (unsigned int c = 0; c < attribute->count * 4; c++) color[c] = (float)temp[c] / 65535.0f;
            }
            else if (CHECK_TYPES(attribute, cgltf_component_type_r_32f, cgltf_type_vec4)) {
                printf("\t\tloading color:\n");
                LOAD_ATTRIBUTE(attribute, 4, float, color);
            }*/
            else
                printf("MODEL: [%s] Color attribute data format not supported", fileName_);



            break;
        }
        // NOTE: Attributes related to animations are processed separately
    }



    if (primitive.indices)
    {
        cgltf_accessor* attribute = primitive.indices;
        indices_.resize(attribute->count);

        size_t res = cgltf_accessor_unpack_indices(attribute, indices_.data(), sizeof(uint32_t), attribute->count);

        if (res == 0)
            throw std::logic_error("failed loading indices");

        prevCap = indices_.capacity();
        md.SetIndices(std::move(indices_));
        indices_.reserve(prevCap);

        printf("\t\tindices loaded succesfully\n");

        md.CalcTriangleCount();
        printf("\t\thave set triangles count to %i \n", md.GetTriangleCount());


        //for (int idc = prevSize; idc < indices.size(); idc++) {
        //    indices[idc] += totalVerticesCount_;
        //}
    }
    else
        printf("MODEL: [%s] No indices data!", fileName_);

#undef CHECK_TYPES
}

void MeshLoader::Free() const {
    delete[] fileData_.release();
    cgltf_free(meshData_);
}

void MeshLoader::LoadMesh(const char* fileName, Mesh& outMesh) const {

    //Function initially implemented by Wilhem Barbier(@wbrbr), with modifications by Tyler Bezera(@gamerfiend)

    fileName_ = fileName;
    dstMesh_ = &outMesh;

    OpenFile();

    ParseTree();

    // Load meshes data
    //----------------------------------------------------------------------------------------------------
    for (uint32_t i = 0, meshIndex = 0; i < meshData_->nodes_count; i++) {

        if (meshData_->nodes[i].mesh == nullptr) {
            printf("object %s has no mesh, skipping...\n", meshData_->nodes[i].name);
            continue;
        }

        cgltf_mesh& meshRef = *(meshData_->nodes[i].mesh);

        // NOTE: meshIndex accumulates primitives
        printf("loading mesh: %i\n", i + 1);
        printf("\t%s from object %s\n", meshRef.name, meshData_->nodes[i].name);

        size_t totalvertices_Count_ = 0;

        for (uint32_t p = 0; p < meshRef.primitives_count; p++) {

            if (meshRef.primitives[p].type != cgltf_primitive_type_triangles) {
                printf("\tprimitive %i type is not triangle, skipping... \n", p);
                continue;
            }

            totalVerticesCount_ = vertices_.size() / 3;

            MeshData md;

            printf("\tloading primitive %i\n", p + 1);

            printf("\tit has %zu attributes\n", meshRef.primitives[p].attributes_count);
            printf("\tit has %zu indices (%zu triangles)\n", meshRef.primitives[p].indices->count, meshRef.primitives[p].indices->count / 3);


            printf("got meshData %u: \n", meshIndex);


            LoadAttributes(md, meshRef, meshRef.primitives[p], meshData_->nodes[i]);


            md.SetAttrAvailability(attrMask_);


            //printf("mesh (primitive) has been loaded, attributes are: \n");
            //md.PrintAttributes();

            outMesh.AddMesh(std::move(md));

            // Move to next mesh
            meshIndex++;

        }
    }


    printf("mesh has been loaded, attributes are: \n");
    Free();

}