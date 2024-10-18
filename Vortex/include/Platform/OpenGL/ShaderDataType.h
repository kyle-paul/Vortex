#pragma once
#include <glad/glad.h>
#include "Vortex/Core/Assert.h"

namespace Vortex
{
    enum class ShaderDataType
    {
        None=0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType type) {
        switch(type) {
            case ShaderDataType::Float:     return 4;         // 4 byte - > 32 bit (float32)
            case ShaderDataType::Float2:    return 4 * 2;   
            case ShaderDataType::Float3:    return 4 * 3;
            case ShaderDataType::Float4:    return 4 * 4;
            case ShaderDataType::Mat3:      return 4 * 3 * 3;
            case ShaderDataType::Mat4:      return 4 * 4 * 4;
            case ShaderDataType::Int:       return 4;
            case ShaderDataType::Int2:      return 4 * 2;
            case ShaderDataType::Int3:      return 4 * 3;
            case ShaderDataType::Int4:      return 4 * 4;
            case ShaderDataType::Bool:      return 1;
        }

        VX_CORE_ASSERT(false, "Unknowned or Undefined ShaderDataType!");
        return 0;
    };

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch(type)
		{
			case ShaderDataType::Float : return GL_FLOAT;
			case ShaderDataType::Float2 : return GL_FLOAT;
			case ShaderDataType::Float3 : return GL_FLOAT;
			case ShaderDataType::Float4 : return GL_FLOAT;
		}
		VX_CORE_ASSERT(false, "Unknowned or Undefined type in ShaderDataTypeToOpenGLBaseType!");
        return 0;
	}
}
