#pragma once

#include "Vortex.h"
#include "Level.h"

class GameLayer : public Vortex::Layer
{
public:
    GameLayer();
    ~GameLayer();

    virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Vortex::TimeStep ts) override;
	void OnEvent(Vortex::Event& e) override;

    virtual void OnImGuiRender() override;

private:
    void CreateCamera(uint32_t width, uint32_t height);
    bool OnWindowResize(Vortex::WindowResizeEvent &event);
    bool OnMouseButtonPressedEvent(Vortex::MouseButtonPressedEvent &event);

private:
    // usually with different states / options
    // that we have to choose => use enum class
    // struct is used for a data structure (dictionary)
    // holding a bunch of properties
    enum class GameState
    {
        Play=0, MainMenu=1, GameOver=2
    };
    GameState m_State = GameState::MainMenu;
    Vortex::Scope<Vortex::OrthographicCamera> m_Camera;
    Level m_Level;

    float m_Time = 0.0f;
    float m_Blink = false;  

    Vortex::ImGuiComponents m_ImGuiComponents;
};