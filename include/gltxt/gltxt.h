#pragma once

#include <gltxt/character.h>
#include <gltxt/mesh.h>
#include <gltxt/label.h>
#include <gltxt/font.h>

namespace gltxt
{
    void Init();
    void Shutdown();
    void PrepareRender(const float* viewProjMat);

    Label CreateLabelFromFont(const char* txt, const char* fontname);
    
    /**
     * @brief Loads a font's data to memory.
     * 
     * @param name The name that the font can be found by later.
     * @param path The path of the file to load the font from.
     * @param pixelHeight The height of the font to be loaded in pixels.
     */
    void LoadFontFromFile(const char* name, const char* path, int pixelHeight);

    /**
     * @brief  Loads a font's data to memory.
     * 
     * @param name The name that the font can be found by later.
     * @param data The data of the font file.
     * @param count The number of bytes of the data.
     * @param pixelHeight The height of the font to be loaded in pixels.
     */
    void LoadFontFromMemory(const char* name, const unsigned char* data, size_t count, int pixelHeight);
}