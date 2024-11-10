#include "Graphics/RenderMulti.h"
#include "Graphics/UniformBuffer.h"
#include "Vortex/Scene/Component.h"

namespace Vortex
{
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
        float TilingFactor;
        int EntityID;
    };

    struct RendererManager
    {
        // This settings is only for quad rendering
        static const uint32_t MaxQuads = 20000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 32;

        Ref<VertexArray> VA;
        Ref<VertexBuffer> VB;
        Ref<Shader> Shad;
        Ref<Texture2D> WhiteTexture;

        uint32_t QuadIndexCount = 0;
        QuadVertex* VBB = nullptr;
        QuadVertex* VBP = nullptr;

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1; // 0 for the white texture;

        glm::vec4 VertexPos[4];
        RenderMulti::Statistics Stats;

        struct CameraData
        {
            glm::mat4 ViewProjection;
        };

        CameraData CameraBuffer;
        Ref<UniformBuffer> CameraUniformBuffer;
    };

    static RendererManager *manager;

    void RenderMulti::Init()
    {
        manager = new RendererManager();

        // Initialize Vertex Array
        manager->VA = VertexArray::Create();

        // Initialize Vertex Buffer
        manager->VB = VertexBuffer::Create(manager->MaxQuads * sizeof(QuadVertex));
        manager->VB->SetLayout({
            { ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float4, "a_Color"        },
			{ ShaderDataType::Float2, "a_TexCoord"     },
			{ ShaderDataType::Float,  "a_TexIndex"     },
			{ ShaderDataType::Float,  "a_TilingFactor" },
			{ ShaderDataType::Int,    "a_EntityID"     },
        });
        manager->VA->AddVertexBuffer(manager->VB);
        manager->VBB = new QuadVertex[manager->MaxVertices];

        // Initialize Index Buffer
        uint32_t *indices = new uint32_t[manager->MaxIndices];
        uint32_t offset = 0;
        for (uint32_t i=0; i < manager->MaxIndices; i += 6)
        {
            indices[i + 0] = 0 + offset;
            indices[i + 1] = 1 + offset;
            indices[i + 2] = 2 + offset;

            indices[i + 3] = 2 + offset;
            indices[i + 4] = 3 + offset;
            indices[i + 5] = 0 + offset;

            offset += 4;
        }
        Ref<IndexBuffer> IB = IndexBuffer::Create(indices, manager->MaxIndices);
        manager->VA->SetIndexBuffer(IB);
        delete[] indices;

        // Initialize Texture
        manager->WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        manager->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        int32_t samplers[manager->MaxTextureSlots];
        for (uint32_t i=0; i < manager->MaxTextureSlots; i++)
        {
            samplers[i] = i;
        }

        // Intialize Shader
        manager->Shad = Shader::Create("QuadShader", "assets/Shaders/Multi.glsl");
        manager->TextureSlots[0] = manager->WhiteTexture;

        // Set the position for each vertex of a quad
        manager->VertexPos[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        manager->VertexPos[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        manager->VertexPos[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        manager->VertexPos[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
        
        manager->CameraUniformBuffer = UniformBuffer::Create(sizeof(RendererManager::CameraData), 0);
    }

    void RenderMulti::Shutdown()
    {
        delete[] manager->VBB;
        delete manager;
    }

    void RenderMulti::BeginScene(const Camera &camera, const glm::mat4 &transform)
    {
        manager->CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
        manager->CameraUniformBuffer->SetData(&manager->CameraBuffer, sizeof(RendererManager::CameraData));
        StartBatch();
    }

    void RenderMulti::BeginScene(const EditorCamera &camera)
    {
        manager->CameraBuffer.ViewProjection = camera.GetViewProjection();
		manager->CameraUniformBuffer->SetData(&manager->CameraBuffer, sizeof(RendererManager::CameraData));
        StartBatch();
    }

    void RenderMulti::NextBatch()
    {
        Flush();
		StartBatch();
    }

    void RenderMulti::StartBatch()
    {
        manager->QuadIndexCount = 0;
        manager->VBP = manager->VBB;
        manager->TextureSlotIndex = 1;
    }

    void RenderMulti::Flush()
    {
        if (manager->QuadIndexCount == 0)
            return; // Nothing to draw
        
        uint32_t dataSize = (uint32_t)((uint8_t*)manager->VBP - (uint8_t*)manager->VBB);
        manager->VB->SetData(manager->VBB, dataSize);

        // Bind Textures
        for (uint32_t i=0; i < manager->TextureSlotIndex; i++)
        {
            manager->TextureSlots[i]->Bind(i);
        }
        manager->Shad->Bind();
        RenderCommand::DrawIndexedMulti(manager->VA, manager->QuadIndexCount);
        manager->Stats.DrawCalls++;
    }

    void RenderMulti::DrawShape(const int &batch, const glm::mat4 &transform, const glm::vec4 &color, const int EntityID, 
                                const Ref<Texture2D> &texture, const float TilingFactor)
    {
        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        if (manager->QuadIndexCount >= RendererManager::MaxIndices)
            NextBatch();

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < manager->TextureSlotIndex; i++)
        {
            if (*manager->TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
        }

        if (textureIndex == 0.0f)
        {
            if (manager->TextureSlotIndex >= RendererManager::MaxTextureSlots)
            {
                NextBatch();
            }
            textureIndex = (float)manager->TextureSlotIndex;
            manager->TextureSlots[manager->TextureSlotIndex] = texture;
			manager->TextureSlotIndex++;
        }
        

        for (size_t i=0; i < quadVertexCount; i++)
        {
            manager->VBP->Position = transform * manager->VertexPos[i];
            manager->VBP->Color = color;
            manager->VBP->TexCoord = textureCoords[i];
            manager->VBP->TexIndex = textureIndex;
            manager->VBP->TilingFactor = TilingFactor;
			manager->VBP->EntityID = EntityID;
			manager->VBP++;
        }

        manager->QuadIndexCount += 6;
        manager->Stats.QuadCount++;
    }

    void RenderMulti::ResetStats()
	{
		memset(&manager->Stats, 0, sizeof(Statistics));
	}

	RenderMulti::Statistics RenderMulti::GetStats()
	{
		return manager->Stats;
	}

    void RenderMulti::EndScene()
	{
		Flush();
	}
}