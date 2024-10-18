#pragma once

#include "Graphics/Buffer.h"

namespace Vortex
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(float *vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const override;
		virtual void Unbind() const override;

        virtual void SetLayout(const BufferLayout &layout) override {
            m_BufferLayout = layout;
        }
        virtual const BufferLayout& GetLayout() const override {
            return m_BufferLayout;
        }

    private:
        uint32_t m_VertexBufferID;
        BufferLayout m_BufferLayout;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t *indices, uint32_t size);
        virtual ~OpenGLIndexBuffer();

        virtual uint32_t GetCount() const { return m_VertexCount; } 
        virtual void Bind() const override;
		virtual void Unbind() const override;
    private:
        uint32_t m_IndexBufferID;
        uint32_t m_VertexCount;
    };
}