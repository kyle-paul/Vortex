#pragma once
#include "Vortex/Core/Core.h"
#include "Graphics/Buffer.h"

namespace Vortex
{
    class VertexArray
    {
    public:
        virtual ~VertexArray() {}
        
        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        virtual void AddVertexBuffer(const Ref<VertexBuffer> &vertex_buffer) = 0;
        virtual void SetIndexBuffer(const Ref<IndexBuffer> &index_buffer) = 0;

        virtual const std::vector<Ref<VertexBuffer>> GetVertexBuffer() const = 0;
        virtual const Ref<IndexBuffer> GetIndexBuffer() const = 0;

        virtual const uint32_t GetVAID() const = 0;

        static Ref<VertexArray> Create();

    private:

};
}