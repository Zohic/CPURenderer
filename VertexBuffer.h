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
                printf("replacing vertex: \n");
                vert.Print();
                data[mat][iterationData[mat].first] = std::move(vert);
                printf("replaced vertex\n");
                
            }
            else {
                printf("adding  vertex: \n");
                vert.Print();
                data[mat].push_back(std::move(vert));
                printf("added vertex\n");
            }
                
            
        }

        void Clear() override {
            for (const auto& [mat, vec] : data) {
                iterationData[mat].first = 0;
                iterationData[mat].second = vec.size();
            }
            dataIterator = data.cbegin();
            vectorIterator = (*dataIterator).second.cbegin();
            endOfBuffer = false;
        }

        bool End() const override{
            printf("eof: %i\n", endOfBuffer);
            return endOfBuffer;
        }

        const VertexData& GetVert() override{
            if (endOfBuffer)
                throw std::exception("GetVert() when End() is true");

            printf("get vert\n");

            const VertexData& t = *vectorIterator;
            vectorIterator++;

            printf("\tgot vert\n");
            t.Print();

            currentMat = (*dataIterator).first;

            if (vectorIterator == (*dataIterator).second.cend()) {
                dataIterator++;

                if (dataIterator != data.cend())
                    vectorIterator = (*dataIterator).second.cbegin();
                else
                    endOfBuffer = true;
            }


            printf("\treturnd vert\n");

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