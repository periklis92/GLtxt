#pragma once

#include <gltxt/font.h>
#include <gltxt/mesh.h>
#include <cstring>

namespace gltxt
{
    class Label
    {
    public:
        Label(const char* txt, const Font* font, const Mesh* mesh,const float pixelSize[2]);
        void Render();

    private:
        const char* mTxt;
        const Font* mFont;
        const Mesh* mMesh;
        float mModel[4][4];
        float mPixelSize[2];

    public:
        inline void SetModel(const float* modelMat4) { memcpy(&mModel, modelMat4, 16 * sizeof(float)); }
        inline float* GetModel() { return &mModel[0][0]; }
        inline void GetSizePixels(float* w, float* h) { *w = mPixelSize[0]; *h = mPixelSize[1]; }
    };    
}