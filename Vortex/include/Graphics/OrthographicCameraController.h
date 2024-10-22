#pragma once

#include "Graphics/OrthographicCamera.h"
#include "Vortex/Core/TimeStep.h"
#include "Vortex/Events/ApplicationEvent.h"
#include "Vortex/Events/MouseEvent.h"

namespace Vortex
{
    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRatio, bool rotation = false);
		~OrthographicCameraController();

        void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

        OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

    private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

    private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		bool m_Rotation;

		OrthographicCamera m_Camera;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };

		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 5.0f;
        float m_CameraRotationSpeed = 180.0f;
    };
}