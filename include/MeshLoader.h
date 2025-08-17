#pragma once


#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#pragma warning(disable:26451)

#include <fstream>
#include <filesystem>
#include "Render.h"
#include "MeshData.h"
#include "Mesh.h"
#include "Transform.h"

#include "cgltf/cgltf.h"

namespace cpuRenderSimple {
    using namespace cpuRenderBase;

    class IMeshLoader {
        virtual void LoadMesh(const char* fileName, Mesh& outMesh) const = 0;
    };

    enum class LoadMethod {
        C_FILE,
        CXX_STREAM
    };

    template<LoadMethod lm>
    char* LoadFileData(const char* fileName, size_t* dataSize);

#ifdef CPU_RENDER_IMPLEMENTATION

    template<>
    char* LoadFileData<LoadMethod::C_FILE>(const char* fileName, size_t* dataSize)
    {
        std::cout << "current path: " << std::filesystem::current_path() << "\n";

        char* data = nullptr;
        
        if (!fileName) {
            printf("File name provided is not valid: %s", fileName);
            return data;
        }

        FILE* fl;
        *dataSize = 0;

        try {
            printf("trying to open file %s\n", fileName);
            fl = fopen(fileName, "rb");
        }
        catch (std::exception& exc) {
            printf("failed opening file: %s\n", fileName);
        }


        if (fl == nullptr) {
            printf("failed opening file: %s\n", fileName);
            return data;
        }

        fseek(fl, 0, SEEK_END);
        size_t size = ftell(fl);//fileStream.tellg();
        rewind(fl);

        if (size == 0) {
            printf("invalid file size (0): %s\n", fileName);
            return data;
        }

        data = new (std::nothrow) char[size];

        if (!data) {
            printf("failed to allocate memory for file data: %s", fileName);
            return nullptr;
        }

        fread(data, 1, size, fl);

        size_t count = ftell(fl);
        *dataSize = count;

        if ((*dataSize) != size)
            std::cout << "file partially loaded: " << dataSize << " bytes out of " << count << '\n';
        else std::cout << "file successfuly loaded: " << fileName << '\n';

        fclose(fl);
        return data;
    }

    template<>
    char* LoadFileData<LoadMethod::CXX_STREAM>(const char* fileName, size_t* dataSize) {
        
        char* data = nullptr;
        std::cout << "current path: " << std::filesystem::current_path() << "\n";

        if (!fileName) {
            printf("File name provided is not valid: %s", fileName);
            return data;
        }

        std::ifstream fileStream;
        *dataSize = 0;

        try {
            printf("trying to open file %s", fileName);
            fileStream.open(fileName, std::ios::in | std::ios::binary);
        }
        catch (std::exception& exc) {
            printf("failed opening file: %s\n", fileName);
        }


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

#endif //CPU_RENDER_IMPLEMENTATION


    class MeshLoader final: IMeshLoader {

    private:
        mutable size_t dataSize_ = 0;
        mutable std::unique_ptr<char> fileData_;


        mutable Mesh* dstMesh_ = nullptr;
        mutable const char* fileName_ = nullptr;

        mutable std::vector<float> vertices_;
        mutable std::vector<float> normal_;
        mutable std::vector<float> tangent_;
        mutable std::vector<float> texcoord_;
        mutable std::vector<float> color_;
        mutable std::vector<uint32_t> indices_;

        mutable uint8_t attrMask_ = 0ui64;

        mutable cgltf_data* meshData_ = nullptr;

        mutable size_t totalVerticesCount_ = 0;

        void OpenFile() const;

        void ParseTree() const;

        void FillAttribute(const cgltf_accessor* const accesor, uint32_t numComp, std::vector<float>& dstVec) const;

        void TransformVertices(const cgltf_node& node) const;

        void LoadAttributes(MeshData& md, cgltf_mesh& mesh_ref, const cgltf_primitive& primitive, const cgltf_node& node) const;

        void Free() const;

    public:
        void LoadMesh(const char* fileName, Mesh& outMesh) const override;
    };
}

