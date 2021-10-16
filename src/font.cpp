#include <gltxt/font.h>

#include <iostream>
#include <fstream>
#include <cassert>

#include "internal.h"

namespace gltxt
{
    Font::Font()
        :mAscent(0), mDescent(0), mLineGap(0), mTexWidth(0), mTexHeight(0), mCharacters(nullptr), mNumCharacters(0) {}

        
    Font::Font(int ascent, int descent, int lineGap)
        :mAscent(ascent), mDescent(descent), mLineGap(lineGap), mTexWidth(0), mTexHeight(0), mCharacters(nullptr), mNumCharacters(0) {}

    void Font::SetCharacterSet(Character* characters, int numCharacters)
    {
        if (mCharacters != nullptr)
            delete[] mCharacters;
        mCharacters = new Character[numCharacters];
        mNumCharacters = numCharacters;
        memcpy(mCharacters, characters, numCharacters * sizeof(Character));
    }

    const Character& Font::operator[](size_t index) const
    {
        assert(index < mNumCharacters);
        return mCharacters[index];
    }
    
    const Character& Font::operator[](const char index) const
    {
        assert(index < mNumCharacters);
        return mCharacters[index];
    }
    
    void Font::GetTexSize(int& width, int& height) const
    {
        width = mTexWidth;
        height = mTexHeight;
    }

    unsigned int Font::GetTexHandle() const
    {
        return mTexHandle;
    }
}