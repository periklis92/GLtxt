#pragma once

namespace gltxt
{
    struct CharBoundingBox
    {
        int X0, X1;
        int Y0, Y1;

        CharBoundingBox(int x0,int y0,int x1, int y1)
        :X0(x0), X1(x1), Y0(y0), Y1(y1) { }

        int GetWidth() const;
        int GetHeight() const;
    };

    struct Character
    {
        int Code;
        float AdvanceWidth, LeftSideBearing;
        int X, Y;
        CharBoundingBox BoundingBox;

        Character(int code, float advanceWidth, float leftSideBearing, int x, int y, CharBoundingBox boundingBox);
        Character();
    };
}