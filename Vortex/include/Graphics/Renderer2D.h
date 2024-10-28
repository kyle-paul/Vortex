#pragma once

#include "Graphics/OrthographicCamera.h"
#include "Graphics/Texture.h"
#include "Graphics/Camera.h"
#include "Vortex/Imgui/ImGuiLayer.h"
#include "Graphics/EditorCamera.h"

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

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture);


		static glm::mat4 TransformQuad(const glm::vec3 &position, const float &rotation, const glm::vec2 &size);
		static ImGuiComponents m_ImGuiComponents;
	};
}