#include "GameLayer.h"
#include "Vortex/Imgui/ImGuiLayer.h"


GameLayer::GameLayer() 
    : Layer("GameLayer")
{
    auto &window = Vortex::Application::GetApplication().GetWindow();
    CreateCamera(window.GetWidth(), window.GetHeight());
}

GameLayer::~GameLayer()
{

}

void GameLayer::OnAttach() 
{
   
    m_Level.Init();
}


void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate(Vortex::TimeStep ts)
{
    m_Time += ts;
	if ((int)(m_Time * 10.0f) % 8 > 4)
		m_Blink = !m_Blink;

    if (m_Level.IsGameOver())
		m_State = GameState::GameOver;

    const auto& playerPos = m_Level.GetPlayer().GetPosition();
	m_Camera->SetPosition({ playerPos.x, playerPos.y, 0.0f });
    
    Vortex::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1 });
	Vortex::RenderCommand::ClearBufferBit();

    Vortex::Renderer2DGame::BeginScene(*m_Camera);
    m_Level.OnRender();
    Vortex::Renderer2DGame::EndScene();
}

void GameLayer::OnEvent(Vortex::Event& event)
{
    Vortex::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Vortex::WindowResizeEvent>(VX_BIND_EVENT_FN(GameLayer::OnWindowResize));
    dispatcher.Dispatch<Vortex::MouseButtonPressedEvent>(VX_BIND_EVENT_FN(GameLayer::OnMouseButtonPressedEvent));
}

void GameLayer::OnImGuiRender()
{

}

bool GameLayer::OnMouseButtonPressedEvent(Vortex::MouseButtonPressedEvent &event)
{
    m_State = GameState::Play;
    return false;
}

bool GameLayer::OnWindowResize(Vortex::WindowResizeEvent &event)
{
    CreateCamera(event.GetWidth(), event.GetHeight());
    return false;
}


void GameLayer::CreateCamera(uint32_t width, uint32_t height)
{
    // By using this, you would have no
    // ability of zooming in and out the window
    // just resize the canvas when window size changed
    float aspect_ratio = (float)width / (float)height;
    float camWidth = 8.0f;
    float bottom = -camWidth;
    float top = camWidth;
    float left = bottom * aspect_ratio;
    float right = top * aspect_ratio;
    m_Camera = Vortex::CreateScope<Vortex::OrthographicCamera>(left, right, bottom, top);
}