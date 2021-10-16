#pragma once

#include <gltxt/font.h>
#include <gltxt/mesh.h>
#include <string.h>

namespace gltxt
{
    class Label
    {
    public:
        Label(const Font* font, const Mesh* mesh,const float pixelSize[2]);
        void Render();

    private:
        const Font* mFont;
        const Mesh* mMesh;
        float mModel[4][4];
        float mPixelSize[2];

    public:
        inline void SetModel(const float* modelMat4) { memcpy(&mModel, modelMat4, 16 * sizeof(float)); }
        inline const float* GetModel() const { return &mModel[0][0]; }
        inline void GetSizePixels(float* w, float* h) const { *w = mPixelSize[0]; *h = mPixelSize[1]; }
    };    
}