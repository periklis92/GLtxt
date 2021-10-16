#pragma once

#include <vector>
#include <string>

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

        friend class Mesh;
    private:
        Font();
        Font(int ascent, int descent, int lineGap);
        void AddCharacter(Character character);

    public:
        static Font LoadFontFromFile(const std::string& path, int pixelHeight = 14);
        static Font LoadFontFromMemory(const unsigned char* data, size_t count, int pixelHeight = 14);

    public:
        unsigned int mTexHandle{0};

    private:
        int mAscent, mDescent, mLineGap;
        int mTexWidth, mTexHeight;
        std::vector<Character> mCharacters;
    };
}