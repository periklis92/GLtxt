#pragma once


#ifdef GLTXT_DLL
#ifdef _MSC_VER
#ifdef GLTXT_DLL_EXPORT
#       define GLTXT_API __declspec(dllexport)
#   else
#       define GLTXT_API __declspec(dllimport)
#   endif
#elif __GNUC__
#ifdef GLTXT_DLL_EXPORT
#       define GLTXT_API __attribute__((visibility("default")))
#   else
#       define GLTXT_API
#   endif
#else
#define GLTXT_API
#endif
#else
#define GLTXT_API
#endif

#include <gltxt/character.h>
#include <gltxt/mesh.h>
#include <gltxt/label.h>
#include <gltxt/font.h>
#include <vector>
#include <string>

namespace gltxt
{
    void Init();
    void Shutdown();
    void PrepareRender(const float* viewProjMat);

    Label CreateLabelFromFont(const std::string& txt, const std::string& fontname);
    
    /**
     * @brief Loads a font's data to memory.
     * 
     * @param name The name that the font can be found by later.
     * @param path The path of the file to load the font from.
     * @param pixelHeight The height of the font to be loaded in pixels.
     */
    void LoadFontFromFile(const std::string& name, const std::string& path, int pixelHeight);

    /**
     * @brief  Loads a font's data to memory.
     * 
     * @param name The name that the font can be found by later.
     * @param data The data of the font file.
     * @param count The number of bytes of the data.
     * @param pixelHeight The height of the font to be loaded in pixels.
     */
    void LoadFontFromMemory(const std::string& name, const unsigned char* data, size_t count, int pixelHeight);
}