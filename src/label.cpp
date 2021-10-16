#include <gltxt/label.h>
#include <string.h>
#include "internal.h"

namespace gltxt
{
    Label::Label(const Font* font, const Mesh* mesh,const float pixelSize[2])
    {
        mFont = font;
        mMesh = mesh;
        memset(&mModel[0][0], 0, 16 * sizeof(float));
        memcpy(&mPixelSize[0], &pixelSize[0], 2 * sizeof(float));
    }

    void Label::Render()
    {
        Internal::_render(mMesh, mFont, &mModel[0][0]);
    }
}
