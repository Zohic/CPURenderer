#pragma once
#include "Render.h"


namespace cpuRenderSimple {
    using namespace cpuRenderBase;

    class VertexBuffer final: public VertexBufferBase {
        std::unordered_map<const Material*, std::vector<VertexData>> data;
        std::unordered_map<const Material*, std::pair<size_t, size_t>> iterationData;

        std::unordered_map<const Material*, std::vector<VertexData>>::const_iterator dataIterator;
        std::vector<VertexData>::const_iterator vectorIterator;

        bool endOfBuffer = false;
        const Material* currentMat = nullptr;

    public:
        VertexBuffer() {
            
        }

        void InsertVertex(VertexData& vert, const Material* const mat) override {
            if (iterationData[mat].first < iterationData[mat].second) {
                //for some reason it is still required to use std::move even though vert is a rvalue refernce
                printf("replacing vertex (%i, %i): \n", iterationData[mat].first, iterationData[mat].second);
                data[mat][iterationData[mat].first].Print();
                data[mat][iterationData[mat].first] = std::move(vert);
                data[mat][iterationData[mat].first].Print();
                printf("replaced vertex (%i, %i)\n", iterationData[mat].first, iterationData[mat].second);
                iterationData[mat].first++;

            }
            else {
                printf("adding  vertex: \n");
                vert.Print();
                data[mat].push_back(std::move(vert));
                printf("added vertex\n");
            }

        }

        void ResetIterators() override {
            for (const auto& [mat, vec] : data) {
                iterationData[mat].first = 0;
                iterationData[mat].second = vec.size();
            }
            dataIterator = data.cbegin();
            vectorIterator = (*dataIterator).second.cbegin();
            endOfBuffer = false;
        }

        void Clear() override {
            //this should not cause any slowing down when inserting since std::vector should save the capacity
            for (auto& [mat, vec] : data) {
                vec.clear();
            }
        }

        bool End() const override{
            DEBUGPRINT("eof: %i\n", endOfBuffer);
            return endOfBuffer;
        }

        const VertexData& GetVert() override{
            if (endOfBuffer)
                throw std::exception("GetVert() when End() is true");

            DEBUGPRINT("get vert\n");

            const VertexData& t = *vectorIterator;
            vectorIterator++;

            DEBUGPRINT("\tgot vert\n");
            //t.Print();

            currentMat = (*dataIterator).first;

            if (vectorIterator == (*dataIterator).second.cend()) {
                dataIterator++;

                if (dataIterator != data.cend())
                    vectorIterator = (*dataIterator).second.cbegin();
                else
                    endOfBuffer = true;
            }


            DEBUGPRINT("\treturnd vert\n");

            return t;
        }

        const Material& GetMaterial() const override {
            return *currentMat;
        }

        size_t GetVertexCount() const {
            return 23;
        }

    };

}