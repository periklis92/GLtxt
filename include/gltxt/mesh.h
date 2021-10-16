#pragma once

namespace gltxt
{
    struct Vertex
    {
        float mPosition[3];
        float mUV[2];
    };

    class Mesh
    {
    private:
        unsigned int mVertexArray{0}, mVertexBuffer{0}, mElementBuffer{0};
        int mNumIndices{0};

        friend struct Internal;
    };
}