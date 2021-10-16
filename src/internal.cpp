#include "Internal.h"

#include <stddef.h>
#include <gltxt/mesh.h>
#include <gltxt/font.h>
#include "gl.h"

namespace gltxt
{
    static GLuint _programHandle{0}, _vertHandle{0}, _fragHandle{0};
    static GLuint _vboHandle{0}, _elmHandle{0};
    static GLuint _VPLoc{0}, _ModelLoc{0}, _TexLoc{0};
    static GLuint _PosLoc{0}, _UVLoc{0};

    static void _CheckShader(GLuint handle, const char* msg)
    {
        GLint status = 0, logLength = 0;
        glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);
        if (!status)
        {
            GLTXT_LOG_ERR(msg);
            if (logLength > 1)
            {
                char* buffer = new char[logLength + 1];
                glGetShaderInfoLog(handle, logLength, NULL, (GLchar*)buffer);
                GLTXT_LOG_ERR(buffer);
                delete[] buffer;
            }
        }
    }

    static void _CheckProgram(GLuint handle)
    {
        GLint status = 0, logLength = 0;
        glGetProgramiv(handle, GL_LINK_STATUS, &status);
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLength);
        if (!status)
        {
            fprintf(stderr, "gltxtError: failed to link program!\n");
            if (logLength > 1)
            {
                char* buffer = new char[logLength + 1];
                glGetProgramInfoLog(handle, logLength, NULL, (GLchar*)buffer);
                GLTXT_LOG_ERR(buffer);
                delete[] buffer;
            }
        }
    }

    void Internal::_init()
    {
        const char* vertShader = 
            "#version 460\n"
            "layout (location = 0) in vec3 Position;\n"
            "layout (location = 1) in vec2 UV;\n"
            "uniform mat4 ViewProjMatrix;\n"
            "uniform mat4 ModelMatrix;\n"
            "out vec2 Frag_UV;\n"
            "void main() {\n"
            "   Frag_UV = UV;\n"
            "   gl_Position = ViewProjMatrix * ModelMatrix * vec4(Position, 1.f);\n"
            "}";
            
        const char* fragShader = 
            "#version 460\n"
            "in vec2 Frag_UV;\n"
            "uniform sampler2D Texture;\n"
            "out vec4 Color;\n"
            "void main() {\n"
            "   Color = vec4(0, 0, 0, texture(Texture, Frag_UV.st).r);\n"
            "}";

        _vertHandle = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(_vertHandle, 1, &vertShader, NULL);
        glCompileShader(_vertHandle);
        _CheckShader(_vertHandle, "vertex shader");

        _fragHandle = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(_fragHandle, 1, &fragShader, NULL);
        glCompileShader(_fragHandle);
        _CheckShader(_fragHandle, "fragment shader");

        _programHandle = glCreateProgram();
        glAttachShader(_programHandle, _vertHandle);
        glAttachShader(_programHandle, _fragHandle);
        glLinkProgram(_programHandle);
        _CheckProgram(_programHandle);

        _VPLoc = glGetUniformLocation(_programHandle, "ViewProjMatrix");
        _ModelLoc = glGetUniformLocation(_programHandle, "ModelMatrix");
        _TexLoc = glGetUniformLocation(_programHandle, "Texture");
        _PosLoc = glGetAttribLocation(_programHandle, "Position");
        _UVLoc = glGetAttribLocation(_programHandle, "UV");
        glGenBuffers(1, &_vboHandle);
        glGenBuffers(1, &_elmHandle);
    }

    void Internal::_shutdown()
    {
        if (_vboHandle) glDeleteBuffers(1, &_vboHandle);
        if (_elmHandle) glDeleteBuffers(1, &_elmHandle);
        if (_programHandle && _vertHandle) { glDetachShader(_programHandle, _vertHandle); }
        if (_programHandle && _fragHandle) { glDetachShader(_programHandle, _fragHandle); }
        if (_vertHandle) { glDeleteShader(_vertHandle); }
        if (_fragHandle) { glDeleteShader(_fragHandle); }
        if (_programHandle) { glDeleteProgram(_programHandle); }
    }
    
    unsigned int Internal::_loadFontTexture(unsigned char* data, int width, int height)
    {
        if (!data || !width || !height) return 0;

        unsigned int id = 0;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
        return id;
    }

    void Internal::_prepareRender(const float* viewProjMat)
    {
        glUseProgram(_programHandle);
        glUniform1i(_TexLoc, 0);
        glUniformMatrix4fv(_VPLoc, 1, GL_FALSE, viewProjMat);
    }

    void Internal::_initMeshFromVertices(Mesh* mesh, const Vertex* vertices, int numVertices, const unsigned int* indices, int numIndices)
    {
        mesh->mNumIndices = numIndices;
        glGenVertexArrays(1, &mesh->mVertexArray);
        glGenBuffers(1, &mesh->mVertexBuffer);
        glGenBuffers(1, &mesh->mElementBuffer);
        glBindVertexArray(mesh->mVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->mVertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mElementBuffer);
        glEnableVertexAttribArray(_PosLoc);
        glEnableVertexAttribArray(_UVLoc);

        glVertexAttribPointer(_PosLoc,   3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, mPosition));
        glVertexAttribPointer(_UVLoc,    2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, mUV));
 
        glBufferData(GL_ARRAY_BUFFER,           numVertices * sizeof(Vertex),       (void*)vertices, GL_STREAM_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,   numIndices  * sizeof(unsigned int), (void*)indices,  GL_STREAM_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Internal::_render(const Mesh* mesh, const Font* font, const float* modelMat)
    {
        GLenum last_blend_src_rgb; glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
        GLenum last_blend_dst_rgb; glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
        GLenum last_blend_src_alpha; glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
        GLenum last_blend_dst_alpha; glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
        GLboolean last_blend = glIsEnabled(GL_BLEND);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindTexture(GL_TEXTURE_2D, font->mTexHandle);
        glUniformMatrix4fv(_ModelLoc, 1, false, modelMat);

        glBindVertexArray(mesh->mVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->mVertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mElementBuffer);
        glDrawElements(GL_TRIANGLES, mesh->mNumIndices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
        if (!last_blend) glDisable(GL_BLEND);
    }
    
}