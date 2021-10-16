#include <fstream>
#include <unordered_map>
#include <gltxt/gltxt.h>

#include "gl.h"
#include "Internal.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

namespace gltxt
{
    struct FontLoader
    {
        static Font* LoadFontFromFile(const char* path, int pixelHeight);
        static Font* LoadFontFromMemory(const unsigned char* data, size_t count, int pixelHeight);
    };

    static std::unordered_map<std::string, Font*> _fonts;

    void Init()
    {
        Internal::_init();
    }

    void Shutdown()
    {
        for (auto &&i : _fonts)
        {
            delete i.second;
        }
        
        Internal::_shutdown();
    }

    void PrepareRender(const float* viewProjMat)
    {
        Internal::_prepareRender(viewProjMat);
    }

    void LoadFontFromFile(const char* name, const char* path, int pixelHeight)
    {
        if (_fonts.find(name) != _fonts.end())
            return;

        Font* newFont = FontLoader::LoadFontFromFile(path, pixelHeight);

        if (newFont)
            _fonts[name] = newFont;
    }
    

    void LoadFontFromMemory(const char* name, const unsigned char* data, size_t count, int pixelHeight)
    {
        if (_fonts.find(name) != _fonts.end())
            return;

        Font* newFont = FontLoader::LoadFontFromMemory(data, count, pixelHeight);

        if (newFont)
            _fonts[name] = newFont;
    }

    Label CreateLabelFromFont(const char* txt, const char* fontname)
    {
        if (_fonts.find(fontname) == _fonts.end())
        {
            GLTXT_LOG_ERR("unable to find font!");
            return {nullptr, nullptr, 0};
        }

        Font* font = _fonts[fontname];

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        unsigned int indexOffset = 0;
        float xOffset = 32, yOffset = 32;
        int fontW = 0, fontH = 0;
        font->GetTexSize(fontW, fontH);
        float pixelSize[2] = {0.f, 0.f};
        size_t strSize = strlen(txt);
        for (int i = 0; i < strSize; ++i)
        {
            const Character& character = font->operator[](txt[i]);

            float uvTopLeft[2] = {
                character.X / (float)fontW ,
                character.Y / (float)fontH
            };

            int w = character.BoundingBox.GetWidth(), h = character.BoundingBox.GetHeight();
            
            pixelSize[0] += w; 
            pixelSize[1] = std::max(pixelSize[1], (float)h);

            float adv = character.AdvanceWidth;
            float lsb = character.LeftSideBearing;
            float uvSize[2] = {
                w / (float)fontW,
                h / (float)fontH
            };


            vertices.insert(vertices.end(), 
            {
                {{(float)xOffset + character.BoundingBox.X0,            yOffset + (float)character.BoundingBox.Y1, 0}, {uvTopLeft[0],              uvTopLeft[1] + uvSize[1] }},
                {{(float)xOffset + character.BoundingBox.X0 + (float)w, yOffset + (float)character.BoundingBox.Y1, 0}, {uvTopLeft[0] + uvSize[0],  uvTopLeft[1] + uvSize[1] }},
                {{(float)xOffset + character.BoundingBox.X0 + (float)w, yOffset + (float)character.BoundingBox.Y0, 0}, {uvTopLeft[0] + uvSize[0],  uvTopLeft[1]             }},
                {{(float)xOffset + character.BoundingBox.X0,            yOffset + (float)character.BoundingBox.Y0, 0}, {uvTopLeft[0],              uvTopLeft[1]             }}
            });

            indices.insert(indices.begin(),
            {
                indexOffset + 0, indexOffset + 1, indexOffset + 3, indexOffset + 1, indexOffset + 2, indexOffset + 3 
            });
            indexOffset += 4;

            xOffset += character.AdvanceWidth + character.LeftSideBearing;
        }

        Mesh* mesh = new Mesh();
        Internal::_initMeshFromVertices(mesh, vertices.data(), vertices.size(), indices.data(), indices.size());
        return Label(font, mesh, pixelSize);
    }

    Font* FontLoader::LoadFontFromFile(const char* path, int pixelHeight)
        {
            std::basic_fstream<unsigned char> fontFile(path, std::fstream::in | std::fstream::binary | std::ios::ate);
            if (!fontFile.is_open())
            {
                GLTXT_LOG_ERR("unable to open file\n");
                return nullptr;
            }
            size_t size = fontFile.tellg();
            fontFile.seekg(0, std::ios::beg);
            unsigned char* buffer = new unsigned char[size];
            fontFile.read(buffer, size);
            Font* font = LoadFontFromMemory(buffer, size, pixelHeight);
            delete[] buffer;
            return font;
        }

        Font* FontLoader::LoadFontFromMemory(const unsigned char* data, size_t count, int pixelHeight)
        {
            stbtt_fontinfo fInfo;
            if (!stbtt_InitFont(&fInfo, data, 0))
            {
                GLTXT_LOG_ERR("unable to initialize font from memory!\n");
                return nullptr;
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

            Font* newFont = new Font(ascent, descent, lineGap);

            newFont->mTexWidth = width;
            newFont->mTexHeight = height;
            
            int xOffset = 0, yOffset = 0;
            
            Character characters[128];

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
                
                characters[i] = {i, advance * scale, leftSideBearing * scale, xPos, yPos, CharBoundingBox{x0, y0, x1, y1}};

                xOffset += w + 4;
            }

            newFont->SetCharacterSet(&characters[0], 128);

            newFont->mTexHandle = Internal::_loadFontTexture(bitmap, width, height);
            stbtt_FreeBitmap(bitmap, NULL);
            return newFont;
        }
}