#pragma once
#include "Vortex.h"
#include "Panels/SceneHierarchyPanel.h"


class EditorLayer : public Vortex::Layer
{
public:
	EditorLayer();
	virtual ~EditorLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Vortex::TimeStep ts) override;
	void OnEvent(Vortex::Event& e) override;

    virtual void OnImGuiRender() override;
	
private:
	void ShowDockSpaceApp(bool *p_open);
    
private:
	// camera component
	Vortex::OrthographicCameraController m_CameraController;
	float m_AspectRatio = 1300.0f / 800.0f;
	float m_ZoomLevel = 1.0f;

	// Vertex Array and Buffer
	Vortex::Ref<Vortex::VertexArray> m_SquareVA;

	// Shader control
	Vortex::Ref<Vortex::Shader> m_FlatColorShader;
	Vortex::Ref<Vortex::Texture2D> m_CheckerboardTexture;

	// Imgui control
	Vortex::ImGuiComponents m_ImGuiComponents;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	
	// Imgui layer events
	bool is_ViewPortFocused;
	bool is_ViewPortHovered;

	// Frame buffer
	Vortex::Ref<Vortex::Framebuffer> m_Framebuffer;
	glm::vec2 m_ViewPortSize = {0.0f, 0.0f};

	// Scene - entity
	Vortex::Ref<Vortex::Scene> m_ActiveScene;
	Vortex::Entity CameraEntity, CameraEntity2;
	bool m_PrimaryCamera = true;

	// Scene Hierachy Panel
	Vortex::SceneHierarchyPanel m_SceneHierarchyPanel;
};