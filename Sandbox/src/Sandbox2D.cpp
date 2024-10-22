#include "Sandbox2D.h"
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1300.0f / 800.0f, true)
{
    
}

void Sandbox2D::OnAttach()
{
    // ================================ Object 1 ================================
    m_SquareVA = Vortex::VertexArray::Create();
    
    // 1 Vertex Buffer
    float verticesSquare[4 * 5] = {
        -0.5f, -0.5, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
    };

    Vortex::Ref<Vortex::VertexBuffer> squareVB;
    squareVB.reset(Vortex::VertexBuffer::Create(verticesSquare, sizeof(verticesSquare)));

    // 2 Layout Buffer
    squareVB->SetLayout({
        {  Vortex::ShaderDataType::Float3, "a_Position"  },
        {  Vortex::ShaderDataType::Float2, "a_TextCoord" },
    });
    m_SquareVA->AddVertexBuffer(squareVB);

    // 3. Index BUffer
    uint32_t indicesSquare[6] = { 0, 1, 2, 2, 3, 0 };
    Vortex::Ref<Vortex::IndexBuffer> m_SquareIB;
    m_SquareIB.reset(Vortex::IndexBuffer::Create(indicesSquare, 6)); 
    m_SquareVA->SetIndexBuffer(m_SquareIB);

    // 4. Shader program
    m_FlatColorShader = Vortex::Shader::Create("flat_color_shader", "assets/Shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Vortex::TimeStep ts)
{
    // Key event controller for camera
    m_CameraController.OnUpdate(ts);

    Vortex::RenderCommand::SetClearColor({0, 0, 0, 1});
    Vortex::RenderCommand::ClearBufferBit();

    Vortex::Renderer::BeginScene(m_CameraController.GetCamera());
    
    m_FlatColorShader->Bind();
    m_FlatColorShader->SetFloat4("u_Color", m_SquareColor);
    Vortex::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::mat4(1.0f));
 
    Vortex::Renderer::EndScene();
}

void Sandbox2D::OnEvent(Vortex::Event &event) 
{
    m_CameraController.OnEvent(event);
}


void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}