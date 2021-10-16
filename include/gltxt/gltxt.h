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

namespace gltxt
{
    void Init();
    void Shutdown();
    unsigned int LoadFontTexture(unsigned char* data, int width, int height);
    void PrepareRender(const float* viewProjMat);

    Label& NewLabelFromFont(const char* txt, const Font& font);
}