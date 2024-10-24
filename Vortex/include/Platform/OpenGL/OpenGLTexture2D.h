#pragma once
#include "Vortex/PreHeaders.h"
#include "Graphics/Texture.h"
#include <glad/glad.h>

namespace Vortex
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(uint32_t width, uint32_t height);
        OpenGLTexture2D(const std::string &path);
        virtual ~OpenGLTexture2D();

        virtual inline uint32_t GetWidth() const override { return m_Width; }
        virtual inline uint32_t GetHeight() const override { return m_Height; }
        
        virtual void SetData(void* data, uint32_t size) override;
        virtual void Bind(uint32_t slot = 0) const override;

        virtual bool operator==(const Texture &other) const override { return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID; }

    private:
        unsigned int m_RendererID;
        std::string m_Path;
        uint32_t m_Width;
        uint32_t m_Height;
        GLenum m_InternalFormat, m_DataFormat;
    };
}
