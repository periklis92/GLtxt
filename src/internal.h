#pragma once
#include <stdio.h>
#define GLTXT_LOG_ERR(msg) fprintf(stderr, "gltxtError: %s!\n", msg)

namespace gltxt
{
    class Mesh;
    class Font;
    struct Vertex;

    struct Internal
    {
        static void _init();
        static void _shutdown();
        static unsigned int _loadFontTexture(unsigned char* data, int width, int height);
        static void _prepareRender(const float* viewProjMat);
        static void _initMeshFromVertices(Mesh* mesh, const Vertex* vertices, int numVertices, const unsigned int* indices, int numIndices);
        static void _render(const Mesh* mesh, const Font* font, const float* modelMat);
        
        Internal() = delete;


    };    
}