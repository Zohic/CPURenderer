#pragma once

#define CGLTF_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#pragma warning(disable:26451)

#include <fstream>
#include "Render.h"
#include "cgltf/cgltf.h"

char* LoadFileData(const char* fileName, size_t* dataSize) {

    char* data = nullptr;
    *dataSize = 0;



    if (!fileName) {
        printf("File name provided is not valid: %s", fileName);
        return data;
    }
    std::ifstream fileStream(fileName, std::ios::in | std::ios::binary);

    if (!fileStream.is_open()) {
        printf("failed opening file: %s\n", fileName);
        return data;
    }

    fileStream.seekg(0, std::ios::end);
    size_t size = fileStream.tellg();
    fileStream.seekg(0, std::ios::beg);

    if (size == 0) {
        printf("invalid file size (0): %s\n", fileName);
        return data;
    }

    data = new (std::nothrow) char[size];

    if (!data) {
        printf("failed to allocate memory for file data: %s", fileName);
        return nullptr;
    }

    fileStream.read(data, size);

    size_t count = fileStream.gcount();
    *dataSize = count;

    if ((*dataSize) != size)
        std::cout << "file partially loaded: " << dataSize << " bytes out of " << count << '\n';
    else std::cout << "file successfuly loaded: " << fileName << '\n';


    fileStream.close();

    return data;
}


namespace cpuRenderSimple {
    using namespace cpuRenderBase;

    class MeshLoader final: IMeshLoader {
    public:
        void LoadMesh(const char* fileName, MeshData& outMesh) const override  {

            //Function initially implemented by Wilhem Barbier(@wbrbr), with modifications by Tyler Bezera(@gamerfiend)

            // glTF file loading
            size_t dataSize = 0;
            char* fileData;

            std::cout << "loading file \n";
            fileData = LoadFileData(fileName, &dataSize);

            if (fileData == nullptr) {
                printf("failed to load file (%s) data", fileName);
                throw std::exception(("failed to load file (%s) data"s + fileName).c_str());
            }

            //wasn't working until i default-initialized cgltf_options
            //cgltf_parse later had been returning out_of_memory result (from parse_json inside because it was trying to allocate too much json tokens)
            //but setting options.json_token_count to zero had been causing runtime failing so it must be not the only problem

            cgltf_options options{};

            options.type = cgltf_file_type::cgltf_file_type_invalid;

            //these seem to be never used
            //options.file.read = LoadFileGLTFCallback;
            //options.file.release = ReleaseFileGLTFCallback;

            cgltf_data* data = NULL;
            cgltf_result result = cgltf_parse(&options, fileData, dataSize, &data);


            if (result == cgltf_result_success) {
                if (data->file_type == cgltf_file_type_glb)
                    printf("Model basic data (glb) loaded successfully: %s\n", fileName);
                else if (data->file_type == cgltf_file_type_gltf)
                    printf("Model basic data (glTF) loaded successfully: %s\n", fileName);
                else
                    printf("Model format not recognized: %s\n", fileName);

                printf("Meshes count: %i\n", (unsigned int)data->meshes_count);
                printf("Materials count:  %i (+1 default)\n", (unsigned int)data->materials_count);
                printf("Buffers count: %i\n", (unsigned int)data->buffers_count);
                printf("Images count: %i\n", (unsigned int)data->images_count);
                printf("Textures count: %i\n", (unsigned int)data->textures_count);
            }
            else
                printf("could't parse gltf file: %s, error code: %i", fileName, result);


            // Force reading data buffers (fills buffer_view->buffer->data)
            // NOTE: If an uri is defined to base64 data or external path, it's automatically loaded
            result = cgltf_load_buffers(&options, data, fileName);
            if (result != cgltf_result_success)
                printf("MODEL: [%s] Failed to load mesh/material buffers", fileName);

            int primitivesCount = 0;
            for (unsigned int i = 0; i < data->meshes_count; i++)
                primitivesCount += (int)data->meshes[i].primitives_count;

            //TODO transformation based on nodes form files
            /*
            for (int i = 0; i < data->nodes_count; i++) {
                if (data->nodes[i].mesh != nullptr) {
                    printf("mesh %i: %s\n", i, data->nodes[i].mesh->name);
                    printf("\thas translation: %s\n", data->nodes[i].has_translation ? "yes" : "no");
                    printf("\thas rotation: %s\n", data->nodes[i].has_rotation ? "yes" : "no");
                    printf("\thas scale: %s\n", data->nodes[i].has_scale ? "yes" : "no");
                    printf("\thas matrix: %s\n", data->nodes[i].has_matrix ? "yes" : "no");
                }
            }*/


            printf("MODEL: Meshes: %i\n", (unsigned int)data->meshes_count);
            printf("MODEL: Primitive count: %i\n", primitivesCount);

            std::vector<float> vertices;
            std::vector<float> normal;
            std::vector<float> tangent;
            std::vector<float> texcoord;
            std::vector<float> color;
            std::vector<uint32_t> indices;

            uint8_t attrMask = 0ui64;
            size_t prevSize = 0;

#define LOAD_ATTRIBUTE(accesor, numComp, dataType, dstVec) \
    { \
        prevSize = dstVec.size();\
        dstVec.resize(prevSize + (size_t)accesor->count*numComp);\
        /*printf("\tsize: %u -> %u\n", prevSize, dstVec.size());*/\
        int n = 0; \
        dataType *buffer = (dataType*)((char*)accesor->buffer_view->buffer->data + accesor->buffer_view->offset) + accesor->offset/sizeof(dataType);\
        /*printf("\tbuffer data is at %x\n", (dataType *)accesor->buffer_view->buffer->data);*/\
        /*printf("\tbuffer offset is %u bytes of %u bytes (=%u numbers)\n", accesor->buffer_view->offset, sizeof(dataType), accesor->buffer_view->offset/sizeof(dataType));*/\
        /*printf("\taccesor offset is %u bytes of %u bytes (=%u)\n", accesor->offset, sizeof(dataType), accesor->offset/sizeof(dataType));*/\
        for (unsigned int k = 0; k < accesor->count; k++) \
        {\
            for (int l = 0; l < numComp; l++) \
            {\
                /*printf("setting %i value with %i valuse\n", prevSize + k*numComp + l, n + l);*/\
                dstVec[prevSize + k*numComp + l] = buffer[n + l];\
            }\
            n += (int)(accesor->stride/sizeof(dataType));\
        }\
    }

#define CHECK_TYPES(attr, comp_type, vec_type)\
        (attr->component_type ==comp_type) && (attr->type == vec_type)

            // Load meshes data
            //----------------------------------------------------------------------------------------------------
            for (unsigned int i = 0, meshIndex = 0; i < data->meshes_count; i++)
            {
                // NOTE: meshIndex accumulates primitives
                printf("loading mesh: %i\n", i+1);
                printf("\tname is %s\n", data->meshes[i].name);
                printf("\tit has %u attributes\n", data->meshes[i].primitives[0].attributes_count);
                printf("\tit has %u indices (%u triangles)\n", data->meshes[i].primitives[0].indices->count, data->meshes[i].primitives[0].indices->count/3);
                //printf("\tit has %u primitives\n", data->meshes[i].primitives_count);
                
                //printf("\tit is sparse %s\n", data->meshes[i].primitives[0].indices->is_sparse ? "yes" : "no");

                for (unsigned int p = 0; p < data->meshes[i].primitives_count; p++)
                {
                    size_t prevAttributeSize = vertices.size()/3;

                    printf("\tloading primitive: %i\n", p);
                    if (data->meshes[i].primitives[p].type != cgltf_primitive_type_triangles) continue;

                    for (unsigned int j = 0; j < data->meshes[i].primitives[p].attributes_count; j++)
                    {
                        cgltf_accessor* attribute = data->meshes[i].primitives[p].attributes[j].data;
                        
                        switch (data->meshes[i].primitives[p].attributes[j].type) {
                            
                        case cgltf_attribute_type_position:
                            // WARNING: SPECS: POSITION accessor MUST have its min and max properties defined.
                            
                            if (!CHECK_TYPES(attribute, cgltf_component_type_r_32f, cgltf_type_vec3)) {
                                printf("MODEL: [%s] Vertices attribute data format not supported, use vec3 float", fileName);
                                break;
                            }

                            printf("loading pos:\n");

                            LOAD_ATTRIBUTE(attribute, 3, float, vertices);
                            attrMask |= ATTR_POS_MASK;
                            break;

                        case cgltf_attribute_type_normal:
                            if (!CHECK_TYPES(attribute, cgltf_component_type_r_32f, cgltf_type_vec3)) {
                                printf("MODEL: [%s] Normal attribute data format not supported, use vec3 float", fileName);
                                break;
                            }
                            printf("loading normal:\n");
                            LOAD_ATTRIBUTE(attribute, 3, float, normal);
                            attrMask |= ATTR_NORMAL_MASK;
                            break;

                        case cgltf_attribute_type_tangent:
                            if (!CHECK_TYPES(attribute, cgltf_component_type_r_32f, cgltf_type_vec4)) {
                                printf("MODEL: [%s] Tangent attribute data format not supported, use vec4 float", fileName);
                                break;
                            }
                            printf("loading tangent:\n");
                            LOAD_ATTRIBUTE(attribute, 4, float, tangent);
                            attrMask |= ATTR_TANGENT_MASK;
                            break;

                        case cgltf_attribute_type_texcoord:
                            if (!CHECK_TYPES(attribute, cgltf_component_type_r_32f, cgltf_type_vec2)) {
                                printf("MODEL: [%s] Texcoords attribute data format not supported, use vec2 float", fileName);
                                break;
                            }
                            printf("loading texcoord:\n");
                            LOAD_ATTRIBUTE(attribute, 2, float, texcoord);
                            attrMask |= ATTR_TEXCOORD_MASK;
                            break;

                        case cgltf_attribute_type_color:

                            // WARNING: SPECS: All components of each COLOR_n accessor element MUST be clamped to [0.0, 1.0] range.

                            if (CHECK_TYPES(attribute, cgltf_component_type_r_8u, cgltf_type_vec4)) {

                                std::vector<unsigned char> temp;
                                printf("loading color:\n");
                                LOAD_ATTRIBUTE(attribute, 4, unsigned char, temp);

                                color.resize((int)attribute->count * 4);
                                for (unsigned int c = 0; c < attribute->count * 4; c++) color[c] = (float)temp[c] / 255.0f;

                            }
                            else if (CHECK_TYPES(attribute, cgltf_component_type_r_16u, cgltf_type_vec4)) {

                                std::vector<short> temp;
                                printf("loading color:\n");
                                LOAD_ATTRIBUTE(attribute, 4, unsigned short, temp);

                                color.resize((int)attribute->count * 4);
                                for (unsigned int c = 0; c < attribute->count * 4; c++) color[c] = (float)temp[c] / 65535.0f;
                            }
                            else if (CHECK_TYPES(attribute, cgltf_component_type_r_32f, cgltf_type_vec4)) {
                                printf("loading color:\n");
                                LOAD_ATTRIBUTE(attribute, 4, float, color);
                            }
                            else
                                printf("MODEL: [%s] Color attribute data format not supported", fileName);


                            attrMask |= ATTR_COLOR_MASK;
                            break;
                        }
                        // NOTE: Attributes related to animations are processed separately
                    }

                    

                    if (data->meshes[i].primitives[p].indices != NULL)
                    {
                        cgltf_accessor* attribute = data->meshes[i].primitives[p].indices;
                        
                        size_t prevSize = indices.size();
                        indices.resize(prevSize + attribute->count);


                        size_t res = cgltf_accessor_unpack_indices(attribute, indices.data() + prevSize, sizeof(uint32_t), attribute->count);

                        if (res == 0)
                            throw std::exception("failed loading indices");


                        for (int i = prevSize; i < indices.size(); i++) {
                            indices[i] += prevAttributeSize;
                        }

                        /*if (attribute->component_type == cgltf_component_type_r_16u) {
                            //indices.resize(attribute->count);
                            printf("loading indecies, u16:\n");

                            LOAD_ATTRIBUTE(attribute, 1, unsigned short, indices);
                            printf("prev size for ind is %u\n", prevSizeForIndex);
                            
                        }
                        else if (attribute->component_type == cgltf_component_type_r_32u) {
                            //indices.resize(attribute->count);
                            printf("loading indecies, u32:\n");
                            LOAD_ATTRIBUTE(attribute, 1, unsigned int, indices);
                        }
                        else
                            printf("MODEL: [%s] Indices data format not supported, use u32", fileName);*/
                    }
                    else
                        printf("MODEL: [%s] No indices data!", fileName);

                    //model.meshes[meshIndex].triangleCount = model.meshes[meshIndex].vertexCount / 3;    // Unindexed mesh

                    meshIndex++;       // Move to next mesh

                }
            }

            outMesh.SetAttr(ATTR_POS, std::move(vertices));
            outMesh.SetAttr(ATTR_NORMAL, std::move(normal));
            outMesh.SetAttr(ATTR_TANGENT, std::move(tangent));
            outMesh.SetAttr(ATTR_TEXCOORD, std::move(texcoord));
            outMesh.SetAttr(ATTR_COLOR, std::move(color));
            outMesh.SetTriangleCount(indices.size() / 3);
            outMesh.SetIndices(std::move(indices));


            printf("mesh has been loaded, attributes are: \n");
            
            outMesh.SetAttrAvailability(attrMask);
            outMesh.PrintAttributes();

            delete[] fileData;
            cgltf_free(data);
        }
    };
}
