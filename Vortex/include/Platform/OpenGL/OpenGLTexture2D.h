#pragma once
#include "Vortex/PreHeaders.h"
#include "Graphics/Texture.h"

namespace Vortex
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(const std::string &path);
        virtual ~OpenGLTexture2D();

        virtual inline uint32_t GetWidth() const override { return m_Width; }
        virtual inline uint32_t GetHeight() const override { return m_Height; }
        
        virtual void Bind(uint32_t slot = 0) const override;

    private:
        unsigned int m_RendererID;
        std::string m_Path;
        uint32_t m_Width;
        uint32_t m_Height;
    };
}
