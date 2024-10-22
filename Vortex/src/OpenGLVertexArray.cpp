#include "Platform/OpenGL/OpenGLVertexArray.h"
#include <glad/glad.h>

namespace Vortex
{
    OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1, &m_VertexArrayID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_VertexArrayID);
    }
    
    void OpenGLVertexArray::Bind() const {
        glBindVertexArray(m_VertexArrayID);
    }

    void OpenGLVertexArray::UnBind() const {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertex_buffer)
    {
        VX_CORE_ASSERT(vertex_buffer.GetLayout().GetElements().size(), "The VertexBuffer has no layout");

        glBindVertexArray(m_VertexArrayID);
        vertex_buffer->Bind();

        const auto &layout = vertex_buffer->GetLayout();
        uint32_t index = 0;
		
		for (auto &element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetDimCount(), 			      // The number of element on a row
								  ShaderDataTypeToOpenGLBaseType(element.Type),   // GLtype -> float
								  element.Normalized ? GL_TRUE : GL_FALSE,        // Normalized or not
								  layout.GetStride(),                             // The number of element on a row * 4 bytes 
								  (const void*)(uintptr_t)element.Offset);        // Offset
			index++;
		}

        m_VertexBufferArrays.push_back(vertex_buffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &index_buffer)
    {
        glBindVertexArray(m_VertexArrayID);
        index_buffer->Bind();
        m_IndexBuffer = index_buffer;
    }
}