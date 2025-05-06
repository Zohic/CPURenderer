//#define CPUREN_DEBUG
#include "VertexBuffer.h"

namespace cpuRenderSimple {

    VertexBuffer::VertexBuffer() {

    }

    void VertexBuffer::InsertVertex(VertexData&& vert, const Material* const mat) {

        IterationInfo& iterationInfo = iterationInfo_[mat];
        std::vector<VertexData>& vec = matVertTable_[mat];

        if (iterationInfo.first < iterationInfo.second) {
            vec[iterationInfo.first] = std::move(vert);
            DEBUGPRINT("replaced vertex (%zu, %zu)\n", iterationInfo.first, iterationInfo.second);
            iterationInfo.first++;
        }
        else {
            vec.push_back(std::move(vert));
            iterationInfo.first++;
            iterationInfo.second = iterationInfo.first;
            DEBUGPRINT("added vertex (%zu, %zu): \n", iterationInfo.first, iterationInfo.second);
        }

    }

    void VertexBuffer::FillBuffer(VertexData* const src, size_t count, const Material* const mat) {

    }

    void VertexBuffer::ResetIterators(bool recount) {
        if (recount)
            for (const auto& [mat, vec] : matVertTable_) {
                iterationInfo_[mat].first = 0;
                iterationInfo_[mat].second = vec.size();
            }
        else
            for (const auto& [mat, vec] : matVertTable_) {
                iterationInfo_[mat].first = iterationInfo_[mat].second;
            }


        if (matVertTable_.empty()) {
            endOfBuffer = true;
            return;
        }

        tableIter_ = matVertTable_.cbegin();
        vertexIter_ = (*tableIter_).second.cbegin();
        endOfBuffer = false;

    }

    void VertexBuffer::Clear() {
        //this should not cause any slowing down when inserting since std::vector should save the capacity
        for (auto& [mat, vec] : matVertTable_) {
            vec.clear();
        }
    }

    bool VertexBuffer::End() const {
        DEBUGPRINT("eof: %i\n", endOfBuffer);
        return endOfBuffer;
    }

    const VertexData& VertexBuffer::GetVert() {
        if (endOfBuffer)
            throw std::logic_error("trying to GetVert() when End() is true");

        const VertexData& t = *vertexIter_;
        vertexIter_++;

        currentMat_ = (*tableIter_).first;
        //DEBUGPRINT("\tcurrent vert: %i\n", iterationData[currentMat].first);

        iterationInfo_[currentMat_].first -= 1;

        if (iterationInfo_[currentMat_].first == 0) {
            tableIter_++;

            if (tableIter_ != matVertTable_.cend())
                vertexIter_ = (*tableIter_).second.cbegin();
            else
                endOfBuffer = true;
        }


        DEBUGPRINT("\treturned vert as ref: %zu\n, (%f, %f, %f, %f)\n", iterationInfo_[currentMat_].first, t.GetPos().x(), t.GetPos().y(), t.GetPos().z(), t.GetPos().w());

        return t;
    }

    const VertexData* VertexBuffer::GetVertPtr() {
        if (endOfBuffer)
            throw std::logic_error("trying to GetVert() when End() is true");

        const VertexData* t = &(*vertexIter_);
        vertexIter_++;

        currentMat_ = (*tableIter_).first;
        //DEBUGPRINT("\tcurrent vert: %i\n", iterationData[currentMat].first);

        iterationInfo_[currentMat_].first -= 1;

        if (iterationInfo_[currentMat_].first == 0) {
            tableIter_++;

            if (tableIter_ != matVertTable_.cend())
                vertexIter_ = (*tableIter_).second.cbegin();
            else
                endOfBuffer = true;
        }

        DEBUGPRINT("\treturned vert as ptr: %zu\n, (%f, %f, %f, %f)\n", iterationInfo_[currentMat_].first, t->GetPos().x(), t->GetPos().y(), t->GetPos().z(), t->GetPos().w());

        return t;
    }

    const Material& VertexBuffer::GetMaterial() const {
        return *currentMat_;
    }

    size_t VertexBuffer::GetVertexCount() const {
        size_t num = 0;
        for (const auto& [mat, val] : iterationInfo_) {
            num += val.second;
        }
        return num;
    }
}