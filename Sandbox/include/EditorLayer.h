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

	// File Dialogs
	void LoadScene(); 
	void OpenScene(const std::string &filepath);
	void ClearScene();
	void SaveDefault();
	void SaveSceneAs();

	bool OnKeyPressed(Vortex::KeyPressedEvent& event);

	// Runtime Editor Controller
	void OnScenePlay();
	void OnSceneStop();
	void PlayToolBar();

	void OnDuplicateEntity();
	void RenderViewPort();
    
private:
	// Node Editor
	Vortex::NodePanels m_NodePanels;

	// Play button
	enum class SceneState { Edit = 0, Play = 1 };
	SceneState m_SceneState = SceneState::Edit;
	Vortex::Ref<Vortex::Texture2D> m_IconPlay, m_IconStop;

	// camera component
	Vortex::EditorCamera m_EditorCamera;
	float m_AspectRatio = 1800.0f / 1200.0f;
	float m_ZoomLevel = 1.0f;
	
	// Imgui layer events
	bool is_ViewPortFocused;
	bool is_ViewPortHovered;

	// Frame buffer
	Vortex::Ref<Vortex::Framebuffer> m_Framebuffer;
	glm::vec2 m_ViewPortSize = {0.0f, 0.0f};
	glm::vec2 m_ViewportBounds[2];
	int CurrentPixelData = -1;
	Vortex::Entity MouseSelectedEntity;

	// Scene - entity
	Vortex::Ref<Vortex::Scene> m_ActiveScene, m_EditorScene;
	std::string m_EditorScenePath;

	// Scene Hierachy Panel &  Content Browser
	Vortex::Ref<Vortex::SceneHierarchyPanel> m_SceneHierarchyPanel;
	Vortex::ContentBrowserPanel m_ContentBrowserPanel;

	// Gizmo
	int m_GizmoType = -1;
};