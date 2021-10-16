#pragma once

#include <string>
#include <vector>

namespace gltxt
{
    class Font;

    struct Vertex
    {
        float mPosition[3];
        float mUV[2];
    };

    class Mesh
    {
    public:
        void InitFromVertices(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

    private:
        unsigned int mVertexArray{0}, mVertexBuffer{0}, mElementBuffer{0};
        int mNumIndices{0};
        friend struct Internal;
    };
}