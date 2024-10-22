#include "Sandbox2D.h"
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1300.0f / 800.0f, true)
{
    
}

void Sandbox2D::OnAttach()
{
    m_CheckerboardTexture = Vortex::Texture2D::Create("/home/pc/dev/engine/Sandbox/assets/Textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Vortex::TimeStep ts)
{
    // Key event controller for camera
    m_CameraController.OnUpdate(ts);

    Vortex::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1 });
    Vortex::RenderCommand::ClearBufferBit();

	Vortex::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Vortex::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Vortex::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	Vortex::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);
	Vortex::Renderer2D::EndScene();
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