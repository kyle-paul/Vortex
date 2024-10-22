#include "Vortex/Core/Core.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include <glad/glad.h>

namespace Vortex
{

    // ============ Vertex Buffer ============ 
    OpenGLVertexBuffer::OpenGLVertexBuffer(float *vertices, uint32_t size)
    {
        VX_PROFILE_FUNCTION();

        glCreateBuffers(1, &m_VertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {
        VX_PROFILE_FUNCTION();

        glDeleteBuffers(1, &m_VertexBufferID);
    }

    void OpenGLVertexBuffer::Bind() const {
        VX_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
    }

    void OpenGLVertexBuffer::Unbind() const {
        VX_PROFILE_FUNCTION();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    // ============ Index Buffer ============ 
    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *indices, uint32_t count) 
        : m_VertexCount(count)
    {
        VX_PROFILE_FUNCTION();
        glCreateBuffers(1, &m_IndexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        VX_PROFILE_FUNCTION();
        glDeleteBuffers(1, &m_IndexBufferID);
    }

    void OpenGLIndexBuffer::Bind() const {
        VX_PROFILE_FUNCTION();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
    }

    void OpenGLIndexBuffer::Unbind() const {
        VX_PROFILE_FUNCTION();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}