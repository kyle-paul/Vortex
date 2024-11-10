#pragma once
#include "Vortex/Core/Core.h"
#include "Graphics/Texture.h"
#include "Graphics/Camera.h"
#include "Graphics/EditorCamera.h"
#include "Graphics/Shape.h"
#include "Graphics/Shader.h"
#include "Graphics/RenderCommand.h"

namespace Vortex
{
    class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4 &transform);
		static void BeginScene(EditorCamera& camera);
		static void EndScene();

		static void DrawShape(const Shape &shape, const glm::mat4& transform, const glm::vec4& color, 
							  const int EntityID, const Ref<Texture2D>& texture, const float tilingFactor);

		static void DrawCircle(const glm::mat4& transform,  const glm::vec4 &color, 
							   const float Thickness, const float Fade, const int EntityID);
	};
}