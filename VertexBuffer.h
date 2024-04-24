#pragma once
#include "Render.h"


namespace cpuRenderSimple {
    using namespace cpuRenderBase;

    class VertexBuffer final: public VertexBufferBase {
        std::vector<float> buffer = std::vector<float>();
        size_t vertexCount = 0;

    public:
        VertexBuffer() {
            buffer.reserve(2000);
        }

        inline void InsertValue(float val) override {
            buffer.push_back(val);
        };

        void InsertVertex(const Vec3& vert) override {
            buffer.push_back(vert.x());
            buffer.push_back(vert.y());
            buffer.push_back(vert.z());

            vertexCount += 1;
        }
        void Clear() override {
            buffer.clear();
            vertexCount = 0;
        }

        float GetVertX(size_t ind) const override {
            return buffer[ind * 3];
        }
        float GetVertY(size_t ind) const override {
            return buffer[ind * 3 + 1];
        }
        float GetVertZ(size_t ind) const override {
            return buffer[ind * 3 + 2];
        }

        size_t GetVertexCount() const override {
            return vertexCount;
        }
    };

}