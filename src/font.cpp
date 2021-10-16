#include <gltxt/font.h>

#include <iostream>
#include <fstream>
#include <cassert>

#include "internal.h"


namespace gltxt
{
    Font::Font()
        :mAscent(0), mDescent(0), mLineGap(0), mTexWidth(0), mTexHeight(0), mCharacters() {}

        
    Font::Font(int ascent, int descent, int lineGap)
        :mAscent(ascent), mDescent(descent), mLineGap(lineGap), mTexWidth(0), mTexHeight(0), mCharacters() {}

    void Font::AddCharacter(Character character)
    {
        mCharacters.push_back(character);
    }

    const Character& Font::operator[](size_t index) const
    {
        assert(index < mCharacters.size());
        return mCharacters[index];
    }
    
    const Character& Font::operator[](const char index) const
    {
        assert(index < mCharacters.size());
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