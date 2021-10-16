#include <gltxt/label.h>
#include "internal.h"

namespace gltxt
{
    Label::Label(const char* txt, const Font* font, const Mesh* mesh,const float pixelSize[2])
    {
        mTxt = txt;
        mFont = font;
        mMesh = mesh;
        memset(&mModel[0][0], 0, 16 * sizeof(float));
        memcpy(&mPixelSize[0], &pixelSize[0], 2 * sizeof(float));
    }

    void Label::Render()
    {
        Internal::_renderMesh(*mMesh, &mModel[0][0]);
    }
}
