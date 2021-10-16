#include <gltxt/mesh.h>
#include <gltxt/font.h>
#include <gltxt/character.h>

#include "internal.h"

#ifdef MSVC
#pragma warning (disable: 4267)
#endif

namespace gltxt
{
    void Mesh::InitFromVertices(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
    {
        Internal::_initMeshFromVertices(*this, vertices.data(), vertices.size(), indices.data(), indices.size());
    }
}