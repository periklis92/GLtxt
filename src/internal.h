#pragma once

#include <vector>

namespace gltxt
{
    class Mesh;
    struct Vertex;

    struct Internal
    {
        static void _init();
        static void _shutdown();
        static unsigned int _loadFontTexture(unsigned char* data, int width, int height);
        static void _prepareRender(const float* viewProjMat);
        static void _initMeshFromVertices(Mesh& mesh, const Vertex* vertices, int numVertices, const unsigned int* indices, int numIndices);
        static void _renderMesh(const Mesh& mesh, const float* modelMat);
        
        Internal() = delete;

        
    };    
}