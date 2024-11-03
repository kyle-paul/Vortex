#include "Vortex/Core/Core.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include <glad/glad.h>

namespace Vortex
{
    OpenGLVertexArray::OpenGLVertexArray()
    {
        VX_PROFILE_FUNCTION();
        glCreateVertexArrays(1, &m_VertexArrayID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        VX_PROFILE_FUNCTION();
        glDeleteVertexArrays(1, &m_VertexArrayID);
    }
    
    void OpenGLVertexArray::Bind() const {
        VX_PROFILE_FUNCTION();
        glBindVertexArray(m_VertexArrayID);
    }

    void OpenGLVertexArray::UnBind() const {
        VX_PROFILE_FUNCTION();
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertex_buffer)
    {
        VX_PROFILE_FUNCTION();

        VX_CORE_ASSERT(vertex_buffer.GetLayout().GetElements().size(), "The VertexBuffer has no layout");

        glBindVertexArray(m_VertexArrayID);
        vertex_buffer->Bind();

        const auto &layout = vertex_buffer->GetLayout();
        uint32_t index = 0;

        /*
        glVertexAttribPointer explanation:

        # position, normals, textcoords, face
        
        glCreateBuffers(1, &VBO_VBO_POS_IDID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_POS_ID);
        glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(float), position.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glCreateBuffers(1, &VBO_NORMS_ID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_NORMS_ID);
        glBindBuffer(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glCreateBuffers(1, &VBO_TEX_ID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_TEX_ID);
        glBindBuffer(GL_ARRAY_BUFFER, texture.size() * sizeof(float), texture.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glCreateBuffers(1, &VBO_FACE_ID)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, face);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, face.size() * sizeof(unsigned int), face.data(), GL_STATIC_DRAW);


        # one vertices
        glCreateBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

        glCreateBuffers(1, &VBO_FACE_ID)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, face);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, face.size() * sizeof(unsigned int), face.data(), GL_STATIC_DRAW);
        */
		
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
        VX_PROFILE_FUNCTION();
        
        glBindVertexArray(m_VertexArrayID);
        index_buffer->Bind();
        m_IndexBuffer = index_buffer;
    }
}