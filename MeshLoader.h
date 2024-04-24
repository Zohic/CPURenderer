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
        void LoadMesh(const char* fileName, MeshData& outMesh) const noexcept override  {

            //Function initially implemented by Wilhem Barbier(@wbrbr), with modifications by Tyler Bezera(@gamerfiend)

            // glTF file loading
            size_t dataSize = 0;
            char* fileData;

            std::cout << "loading file \n";
            fileData = LoadFileData(fileName, &dataSize);

            if (fileData == nullptr) {
                printf("failed to load file (%s) data", fileName);
                throw std::exception("");
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

            printf("MODEL: Meshes: %i\n", (unsigned int)data->meshes_count);
            printf("MODEL: Primitive count: %i\n", primitivesCount);

            std::vector<float> vertices;
            std::vector<float> normal;
            std::vector<float> tangent;
            std::vector<float> texcoord;
            std::vector<float> color;
            std::vector<uint32_t> indices;

#define LOAD_ATTRIBUTE(accesor, numComp, dataType, dstVec) \
    { \
        dstVec.resize((size_t)accesor->count*numComp);\
        int n = 0; \
        dataType *buffer = (dataType *)accesor->buffer_view->buffer->data + accesor->buffer_view->offset/sizeof(dataType) + accesor->offset/sizeof(dataType); \
        for (unsigned int k = 0; k < accesor->count; k++) \
        {\
            for (int l = 0; l < numComp; l++) \
            {\
                dstVec[k*numComp + l] = buffer[n + l];\
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

                for (unsigned int p = 0; p < data->meshes[i].primitives_count; p++)
                {
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

                            LOAD_ATTRIBUTE(attribute, 3, float, vertices);
                            break;

                        case cgltf_attribute_type_normal:
                            if (!CHECK_TYPES(attribute, cgltf_component_type_r_32f, cgltf_type_vec3)) {
                                printf("MODEL: [%s] Normal attribute data format not supported, use vec3 float", fileName);
                                break;
                            }

                            LOAD_ATTRIBUTE(attribute, 3, float, normal)
                                break;

                        case cgltf_attribute_type_tangent:
                            if (!CHECK_TYPES(attribute, cgltf_component_type_r_32f, cgltf_type_vec4)) {
                                printf("MODEL: [%s] Tangent attribute data format not supported, use vec4 float", fileName);
                                break;
                            }

                            LOAD_ATTRIBUTE(attribute, 4, float, tangent);
                            break;

                        case cgltf_attribute_type_texcoord:
                            if (!CHECK_TYPES(attribute, cgltf_component_type_r_32f, cgltf_type_vec2)) {
                                printf("MODEL: [%s] Texcoords attribute data format not supported, use vec2 float", fileName);
                                break;
                            }

                            LOAD_ATTRIBUTE(attribute, 2, float, texcoord)
                                break;

                        case cgltf_attribute_type_color:

                            // WARNING: SPECS: All components of each COLOR_n accessor element MUST be clamped to [0.0, 1.0] range.

                            if (CHECK_TYPES(attribute, cgltf_component_type_r_8u, cgltf_type_vec4)) {

                                std::vector<unsigned char> temp;
                                LOAD_ATTRIBUTE(attribute, 4, unsigned char, temp);

                                color.resize((int)attribute->count * 4);
                                for (unsigned int c = 0; c < attribute->count * 4; c++) color[c] = (float)temp[c] / 255.0f;

                            }
                            else if (CHECK_TYPES(attribute, cgltf_component_type_r_16u, cgltf_type_vec4)) {

                                std::vector<short> temp;
                                LOAD_ATTRIBUTE(attribute, 4, unsigned short, temp);

                                color.resize((int)attribute->count * 4);
                                for (unsigned int c = 0; c < attribute->count * 4; c++) color[c] = (float)temp[c] / 65535.0f;
                            }
                            else if (CHECK_TYPES(attribute, cgltf_component_type_r_32f, cgltf_type_vec4)) {

                                LOAD_ATTRIBUTE(attribute, 4, float, color);
                            }
                            else
                                printf("MODEL: [%s] Color attribute data format not supported", fileName);

                            break;
                        }
                        // NOTE: Attributes related to animations are processed separately
                    }


                    if (data->meshes[i].primitives[p].indices != NULL)
                    {
                        cgltf_accessor* attribute = data->meshes[i].primitives[p].indices;

                        outMesh.SetTriangleCount((unsigned int)attribute->count / 3);

                        if (attribute->component_type == cgltf_component_type_r_16u) {
                            indices.resize(attribute->count);
                            LOAD_ATTRIBUTE(attribute, 1, unsigned short, indices);
                        }
                        else if (attribute->component_type == cgltf_component_type_r_32u) {
                            indices.resize(attribute->count);
                            LOAD_ATTRIBUTE(attribute, 1, unsigned int, indices);
                        }
                        else
                            printf("MODEL: [%s] Indices data format not supported, use u32", fileName);
                    }
                    else
                        printf("MODEL: [%s] No indices data!", fileName);
                    //model.meshes[meshIndex].triangleCount = model.meshes[meshIndex].vertexCount / 3;    // Unindexed mesh

                    meshIndex++;       // Move to next mesh

                }
            }

            outMesh.SetList(MeshData::VERTEX_OFFSET, std::move(vertices));
            outMesh.SetList(MeshData::NORMAL_OFFSET, std::move(normal));
            outMesh.SetList(MeshData::TANGENT_OFFSET, std::move(tangent));
            outMesh.SetList(MeshData::TEXCOORD_OFFSET, std::move(texcoord));
            outMesh.SetList(MeshData::COLOR_OFFSET, std::move(color));
            outMesh.SetIndices(std::move(indices));

            delete[] fileData;
            cgltf_free(data);
        }
    };
}
