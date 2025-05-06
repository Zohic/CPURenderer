#pragma once
#include "Render.h"


namespace cpuRenderSimple {
    using namespace cpuRenderBase;
   
    class VertexBuffer final: public VertexBufferBase {
        
        template<typename K, typename V>
        using HashMap = std::unordered_map<K, V>;
        using IterationInfo = std::pair<size_t, size_t>;

        //pointers are used as keys since materials are expected to be stored in a storage 
        //with persistent ownership (addresses wont change)
        HashMap<const Material*, std::vector<VertexData>> matVertTable_;
        HashMap<const Material*, std::vector<VertexData>>::const_iterator tableIter_;

        //first for curent iteration vertex, second for capacity/available vertices
        //used both for insertion and retrieval
        HashMap<const Material*, IterationInfo> iterationInfo_;
        std::vector<VertexData>::const_iterator vertexIter_;

        bool endOfBuffer = false;
        const Material* currentMat_ = nullptr;

    public:
        VertexBuffer();

        void InsertVertex(VertexData&& vert, const Material* const mat) override;

        void FillBuffer(VertexData* const src, size_t count, const Material* const mat);

        void ResetIterators(bool recount) override;

        void Clear() override;

        bool End() const override;

        const VertexData& GetVert() override;
        const VertexData* GetVertPtr() override;

        const Material& GetMaterial() const override;

        size_t GetVertexCount() const;

    };

}