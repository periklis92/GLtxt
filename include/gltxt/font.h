#pragma once

#include <gltxt/character.h>


namespace gltxt
{
    class Font
    {
    public:
        const Character& operator[](size_t index) const;
        const Character& operator[](const char index) const;

        void GetTexSize(int& width, int& height) const;
        unsigned int GetTexHandle() const;
    private:
        Font();
        Font(int ascent, int descent, int lineGap);
        void SetCharacterSet(Character* character, int numCharacters);
        
        friend struct FontLoader;
        friend class Mesh;
        friend struct Internal;
        
    private:
        unsigned int mTexHandle{0};
        int mAscent, mDescent, mLineGap;
        int mTexWidth, mTexHeight;
        Character* mCharacters;
        int mNumCharacters;

    public:
        bool operator==(const Font& other)
        {
            return mTexHandle == other.mTexHandle && mTexHeight == other.mTexHeight && mTexWidth == other.mTexWidth
                && mAscent == other.mAscent && mDescent == other.mDescent && mLineGap == other.mLineGap;
        }
    };    
}
