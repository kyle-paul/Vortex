#include "Vortex/Core/Assert.h"
#include "Graphics/Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Graphics/Renderer.h"

namespace Vortex 
{
    VertexBuffer* VertexBuffer::Create(float *vertices, uint32_t size) {

        // Decide which API backend for rendering
        switch(Renderer::GetAPI()) 
        {
            case RendererAPI::None: {
                VX_CORE_ASSERT(false, "[Vertex Buffer] None API backend is not supported.");
                return nullptr;
            }
            case RendererAPI::OpenGL: {
                return new OpenGLVertexBuffer(vertices, size);
            }
            case RendererAPI::VulCan: {
                VX_CORE_ASSERT(false, "[Vertex Buffer] Vulcan API backend currently is not supported.");
                return nullptr;
            }
            case RendererAPI::DirectX: {
                VX_CORE_ASSERT(false, "[Vertex Buffer] DirectX API backend currently is not supported.");
                return nullptr;
            }
        }
        VX_CORE_ASSERT(false, "[Vertex Buffer] Please select an API backend for rendering. Options are [OpenGL, Vulcan, DirectX]");
        return  nullptr;
    }

    VertexBuffer::~VertexBuffer() {

    }

    IndexBuffer* IndexBuffer::Create(uint32_t *indices, uint32_t count) {

        // Decide which API backend for rendering
        switch(Renderer::GetAPI()) 
        {
            case RendererAPI::None: {
                VX_CORE_ASSERT(false, "[Index Buffer] None API backend is not supported.");
                return nullptr;
            }
            case RendererAPI::OpenGL: {
                return new OpenGLIndexBuffer(indices, count);
            }
            case RendererAPI::VulCan: {
                VX_CORE_ASSERT(false, "[Index Buffer] Vulcan API backend currently is not supported.");
                return nullptr;
            }
            case RendererAPI::DirectX: {
                VX_CORE_ASSERT(false, "[Index Buffer] DirectX API backend currently is not supported.");
                return nullptr;
            }
        }
        VX_CORE_ASSERT(false, "[Index Buffer] Please select an API backend for rendering. Options are [OpenGL, Vulcan, DirectX]");
        return  nullptr;
    }

        IndexBuffer::~IndexBuffer() {
        
    }
}