#pragma once

#include "Graphics/VertexArray.h"

namespace Vortex 
{
    class OpenGLVertexArray : public VertexArray
    {
        public:
            OpenGLVertexArray();
            virtual ~OpenGLVertexArray();

            virtual void Bind() const override;
            virtual void UnBind() const override;

            virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertex_buffer) override;
            virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> &index_buffer) override;

            virtual const std::vector<std::shared_ptr<VertexBuffer>> GetVertexBuffer() const override { return m_VertexBufferArrays; }
            virtual const std::shared_ptr<IndexBuffer> GetIndexBuffer() const override { return m_IndexBuffer; }
        
        private:
            uint32_t m_VertexArrayID;
            std::vector<std::shared_ptr<VertexBuffer>> m_VertexBufferArrays;
            std::shared_ptr<IndexBuffer> m_IndexBuffer;
    };
}