#include <gltxt/font.h>

#include <iostream>
#include <fstream>
#include <cassert>

#include "internal.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"


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

    Font Font::LoadFontFromFile(const std::string& path, int pixelHeight)
    {
        std::basic_fstream<unsigned char> fontFile(path, std::fstream::in | std::fstream::binary | std::ios::ate);
        if (!fontFile.is_open())
        {
            fprintf(stderr, "gltxtError: Unable to open file %s!\n", path.c_str());
            return {};
        }
        size_t size = fontFile.tellg();
        fontFile.seekg(0, std::ios::beg);
        unsigned char* buffer = new unsigned char[size];
        fontFile.read(buffer, size);
        Font font = LoadFontFromMemory(buffer, size, pixelHeight);
        delete[] buffer;
        return font;
    }

    Font Font::LoadFontFromMemory(const unsigned char* data, size_t count, int pixelHeight)
    {
		stbtt_fontinfo fInfo;
        if (!stbtt_InitFont(&fInfo, data, 0))
		{
            fprintf(stderr, "gltxtError: Unable to load font from memory!\n");
			return {};
		}

        int ascent = 0, descent = 0, lineGap = 0;
		stbtt_GetFontVMetrics(&fInfo, &ascent, &descent, &lineGap);
        
        float scale = stbtt_ScaleForPixelHeight(&fInfo, static_cast<float>(pixelHeight));

        ascent = static_cast<int>(ceilf(ascent * scale));
        descent = static_cast<int>(ceilf(descent * scale));

        int width = 2, height = 2;
		int minSize = (int)ceilf(sqrtf(128) * (ascent - descent)) ;
		while(width < minSize)
		{
			width = width << 1;
		}
        height = width;

        unsigned char* bitmap = new unsigned char[width * height];
		memset(bitmap, 0, width * height);

        Font newFont(ascent, descent, lineGap);

        newFont.mTexWidth = width;
        newFont.mTexHeight = height;
        
        int xOffset = 0, yOffset = 0;
        

        for (int i = 0; i < 128; ++i)
        {
            int advance, leftSideBearing;
            int glyph = stbtt_FindGlyphIndex(&fInfo, i);
            stbtt_GetGlyphHMetrics(&fInfo, glyph, &advance, &leftSideBearing);
			int x0, y0, x1, y1;
            stbtt_GetGlyphBitmapBox(&fInfo, glyph, scale, scale, &x0, &y0, &x1, &y1);
            int w = x1 - x0, h = y1 - y0;

            if (xOffset + w >= width)
			{
				yOffset += (ascent - descent);
				xOffset = 1;
			}

			int yPos = ascent +  yOffset;
            int xPos = xOffset;
			int byteOffset = xOffset + (yPos * width);
            assert(xPos < width);
            assert(yPos < height);
            stbtt_MakeGlyphBitmap(&fInfo, bitmap + byteOffset, w, h, width, scale, scale, glyph);
            
            Character newChar{i, advance * scale, leftSideBearing * scale, xPos, yPos, CharBoundingBox{x0, y0, x1, y1}};
            newFont.AddCharacter(newChar);

            xOffset += w + 4;
        }

        newFont.mTexHandle = Internal::_loadFontTexture(bitmap, width, height);
        stbtt_FreeBitmap(bitmap, NULL);
        return newFont;
    }
}