#include "Platform/OpenGL/OpenGLBuffer.h"
#include <glad/glad.h>

namespace Vortex
{

    // ============ Vertex Buffer ============ 
    OpenGLVertexBuffer::OpenGLVertexBuffer(float *vertices, uint32_t size)
    {
        glCreateBuffers(1, &m_VertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {
        glDeleteBuffers(1, &m_VertexBuffer);
    }

    void OpenGLVertexBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    }

    void OpenGLVertexBuffer::Unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    // ============ Index Buffer ============ 
    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *indices, uint32_t count) 
        : m_VertexCount(count)
    {
        glCreateBuffers(1, &m_IndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_IndexBuffer);
    }

    void OpenGLIndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    }

    void OpenGLIndexBuffer::Unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}