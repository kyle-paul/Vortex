#include "Vortex/Core/Core.h"
#include "Graphics/OrthographicCameraController.h"
#include "Vortex/Core/Core.h"
#include "Vortex/Core/Input.h"
#include "Vortex/Core/KeyCodes.h"

namespace Vortex
{
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
        : m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
    {
           
    }

    OrthographicCameraController::~OrthographicCameraController()
    {
        
    }

    void OrthographicCameraController::OnUpdate(TimeStep ts)
    {
		VX_PROFILE_FUNCTION();

        if (Vortex::Input::IsKeyPressed(Vortex::Key::Left)) {
			m_CameraPosition.x -= m_CameraTranslationSpeed * float(ts);
		}

		else if (Vortex::Input::IsKeyPressed(Vortex::Key::Right)) {
			m_CameraPosition.x += m_CameraTranslationSpeed * float(ts);
		}

		if (Vortex::Input::IsKeyPressed(Vortex::Key::Up)) {
			m_CameraPosition.y += m_CameraTranslationSpeed * float(ts);
		}

		else if (Vortex::Input::IsKeyPressed(Vortex::Key::Down)) {
			m_CameraPosition.y -= m_CameraTranslationSpeed * float(ts);
		}


		if (m_Rotation)
		{
			if (Input::IsKeyPressed(Vortex::Key::A)) {
				m_CameraRotation += m_CameraRotationSpeed * ts;
            }

			if (Input::IsKeyPressed(Vortex::Key::D)) {
				m_CameraRotation -= m_CameraRotationSpeed * ts;

            }
			m_Camera.SetRotation(m_CameraRotation);
		}

        m_Camera.SetPosition(m_CameraPosition);
        m_CameraTranslationSpeed = m_ZoomLevel;
    }

    void OrthographicCameraController::OnEvent(Event &event)
    {
		VX_PROFILE_FUNCTION();

        EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(VX_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(VX_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
    {
		VX_PROFILE_FUNCTION();

        m_ZoomLevel -= event.GetYOffset() * 0.25f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		VX_PROFILE_FUNCTION();
		m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}