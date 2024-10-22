// #include "Graphics/Shader.h"
#include "Graphics/VertexArray.h"
#include "Graphics/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Vortex
{
    Ref<Shader> Shader::Create(const std::string& name, const std::string &vertexSource, const std::string &FragmentSource) 
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None: {
                VX_CORE_ASSERT(false, "[Vertex Array] None API backend is not supported.");
                return nullptr;
            }
            case RendererAPI::API::OpenGL: {
                return CreateRef<OpenGLShader>(name, vertexSource, FragmentSource);
            }
            case RendererAPI::API::VulCan: {
                VX_CORE_ASSERT(false, "[Vertex Array] Vulcan API backend currently is not supported.");
                return nullptr;
            }
            case RendererAPI::API::DirectX: {
                VX_CORE_ASSERT(false, "[Vertex Array] DirectX API backend currently is not supported.");
                return nullptr;
            }
        }
        VX_CORE_ASSERT(false, "[Vertex Array] Please select an API backend for rendering. Options are [OpenGL, Vulcan, DirectX]");
        return  nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string &filepath)
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None: {
                VX_CORE_ASSERT(false, "[Vertex Array] None API backend is not supported.");
                return nullptr;
            }
            case RendererAPI::API::OpenGL: {
                return CreateRef<OpenGLShader>(name, filepath);
            }
            case RendererAPI::API::VulCan: {
                VX_CORE_ASSERT(false, "[Vertex Array] Vulcan API backend currently is not supported.");
                return nullptr;
            }
            case RendererAPI::API::DirectX: {
                VX_CORE_ASSERT(false, "[Vertex Array] DirectX API backend currently is not supported.");
                return nullptr;
            }
        }
        VX_CORE_ASSERT(false, "[Vertex Array] Please select an API backend for rendering. Options are [OpenGL, Vulcan, DirectX]");
        return  nullptr;
    }

    void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		VX_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	Vortex::Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(name, filepath); 
		Add(name, shader);
		return shader;
	}

	Vortex::Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		VX_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}