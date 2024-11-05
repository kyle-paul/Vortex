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

        virtual uint32_t GetWidth() const override { return m_Width; }
        virtual uint32_t GetHeight() const override { return m_Height; }
        virtual uint32_t GetRendererID() const override {  return m_RendererID; }
        
        virtual void SetData(void* data, uint32_t size) override;
        virtual void Bind(uint32_t slot = 0) const override;

        virtual std::string GetPath() const override 
        {
            if (m_Path.size() > 0) return m_Path;
            else return "white texture";
        }

    private:
        unsigned int m_RendererID;
        std::string m_Path;
        uint32_t m_Width;
        uint32_t m_Height;
        GLenum m_InternalFormat, m_DataFormat;
    };
}
