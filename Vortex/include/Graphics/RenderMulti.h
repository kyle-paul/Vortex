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
    class RenderMulti
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const Camera &camera, const glm::mat4 &transform);
        static void BeginScene(const EditorCamera &camera);
        static void EndScene();
        static void Flush();

        static void DrawShape(const int &batch, const glm::mat4 &transform, const glm::vec4 &color, const int EntityID, 
                              const Ref<Texture2D> &texture, const float TilingFactor);        

        struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};

        static void ResetStats();
		static Statistics GetStats();

    private:
        static void StartBatch();
        static void NextBatch();
    };
}