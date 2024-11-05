#pragma once
#include "Vortex.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "NodePanels.h"


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
	void LoadScene(); 
	void OpenScene(const std::string &filepath);
	void ClearScene();
	void SaveSceneAs();
	void SaveDefault();

	bool OnKeyPressed(Vortex::KeyPressedEvent& event);

	void OnScenePlay();
	void OnSceneStop();

	void PlayToolBar();
	void RenderViewPort();
    
private:
	// Node Editor
	Vortex::NodePanels m_NodePanels;

	// Play button
	enum class SceneState
	{
		Edit = 0, Play = 1
	};
	SceneState m_SceneState = SceneState::Edit;
	Vortex::Ref<Vortex::Texture2D> m_IconPlay, m_IconStop;

	// camera component
	Vortex::EditorCamera m_EditorCamera;
	float m_AspectRatio = 1300.0f / 800.0f;
	float m_ZoomLevel = 1.0f;
	
	// Imgui layer events
	bool is_ViewPortFocused;
	bool is_ViewPortHovered;

	// Frame buffer
	Vortex::Ref<Vortex::Framebuffer> m_Framebuffer;
	glm::vec2 m_ViewPortSize = {0.0f, 0.0f};
	glm::vec2 m_ViewportBounds[2]; // object clicking
	int CurrentPixelData = -1;
	Vortex::Entity MouseSelectedEntity;

	// Scene - entity
	Vortex::Ref<Vortex::Scene> m_ActiveScene;
	Vortex::Entity CameraEntity, CameraEntity2;
	bool m_PrimaryCamera = true;

	// Scene Hierachy Panel
	Vortex::Ref<Vortex::SceneHierarchyPanel> m_SceneHierarchyPanel;

	// Gizmo
	int m_GizmoType = -1;

	// Content Browser
	Vortex::ContentBrowserPanel m_ContentBrowserPanel;
};