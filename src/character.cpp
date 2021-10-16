#include <gltxt/character.h>

namespace gltxt
{
    
    int CharBoundingBox::GetWidth() const { return X1 - X0; }
    int CharBoundingBox::GetHeight() const { return Y1 - Y0; }

    Character::Character(int code, float advanceWidth, float leftSideBearing, int x, int y, CharBoundingBox boundingBox)
        :Code(code), AdvanceWidth(advanceWidth), LeftSideBearing(leftSideBearing), BoundingBox(boundingBox),
        X(x), Y(y) { }
}