#include <cstddef>
#include <cstdio>
#include <cstring>
#include <gltxt/gltxt.h>
#include "gl.h"
#include "Internal.h"
#include <algorithm>

namespace gltxt
{
    void Init()
    {
        Internal::_init();
    }

    void Shutdown()
    {
        Internal::_shutdown();
    }
    
    unsigned int LoadFontTexture(unsigned char* data, int width, int height)
    {
        return Internal::_loadFontTexture(data, width, height);
    }

    void PrepareRender(const float* viewProjMat)
    {
        Internal::_prepareRender(viewProjMat);
    }

    Label& NewLabelFromFont(const char* txt, const Font& font)
    {
        size_t sLen = strlen(txt);
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        unsigned int indexOffset = 0;
        float xOffset = 32, yOffset = 32;
        int fontW = 0, fontH = 0;
        font.GetTexSize(fontW, fontH);
        float pixelSize[2] = {0.f, 0.f};

        for (int i = 0; i < sLen; ++i)
        {
            const Character& character = font[txt[i]];

            float uvTopLeft[2] = {
                character.X / (float)fontW ,
                character.Y / (float)fontH
            };

            int w = character.BoundingBox.GetWidth(), h = character.BoundingBox.GetHeight();
            
            pixelSize[0] += w; 
            pixelSize[1] = std::max(pixelSize[1], (float)h);

            float adv = character.AdvanceWidth;
            float lsb = character.LeftSideBearing;
            float uvSize[2] = {
                w / (float)fontW,
                h / (float)fontH
            };


            vertices.insert(vertices.end(), 
            {
                {{(float)xOffset + character.BoundingBox.X0,            yOffset + (float)character.BoundingBox.Y1, 0}, {uvTopLeft[0],              uvTopLeft[1] + uvSize[1] }},
                {{(float)xOffset + character.BoundingBox.X0 + (float)w, yOffset + (float)character.BoundingBox.Y1, 0}, {uvTopLeft[0] + uvSize[0],  uvTopLeft[1] + uvSize[1] }},
                {{(float)xOffset + character.BoundingBox.X0 + (float)w, yOffset + (float)character.BoundingBox.Y0, 0}, {uvTopLeft[0] + uvSize[0],  uvTopLeft[1]             }},
                {{(float)xOffset + character.BoundingBox.X0,            yOffset + (float)character.BoundingBox.Y0, 0}, {uvTopLeft[0],              uvTopLeft[1]             }}
            });

            indices.insert(indices.begin(),
            {
                indexOffset + 0, indexOffset + 1, indexOffset + 3, indexOffset + 1, indexOffset + 2, indexOffset + 3 
            });
            indexOffset += 4;

            xOffset += character.AdvanceWidth + character.LeftSideBearing;
        }

        Mesh* mesh = new Mesh();
        mesh->InitFromVertices(vertices, indices);

        return *new Label(txt, &font, mesh, pixelSize);
    }
}