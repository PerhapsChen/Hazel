#include "hzpch.h"
#include "Hazel/Renderer/PerspectiveCameraController3D.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"


namespace Hazel {

	PerspectiveCameraController3D::PerspectiveCameraController3D(uint32_t viewport_width, uint32_t viewport_height)
		: m_AspectRatio((float)viewport_width / (float)viewport_height), m_Camera(20.0f, m_AspectRatio, 0.1f, 100.0f),
		m_ViewPortWidth(viewport_width), m_ViewPortHeight(viewport_height), m_isFirstMouse(true)
	{
		m_Camera.m_AspectRatio = m_AspectRatio;
		m_LastX = (float)m_ViewPortWidth / 2.0f;
		m_LastY = (float)m_ViewPortHeight / 2.0f;
	}

	void PerspectiveCameraController3D::OnUpdate(Timestep ts)
	{
		HZ_PROFILE_FUNCTION();
	
		if (!Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_RIGHT))
		{
			return;
		}

		auto& Front = m_Camera.m_Front;
		auto& Right = m_Camera.m_Right;
		auto& Up = m_Camera.m_Up;

		float velocity = ts.GetSeconds() * m_CameraTranslationSpeed;

		if (Input::IsKeyPressed(HZ_KEY_A))
		{
			m_CameraPosition -= Right * velocity;
		}
		else if (Input::IsKeyPressed(HZ_KEY_D))
		{
			m_CameraPosition += Right * velocity;
		}

		if (Input::IsKeyPressed(HZ_KEY_W))
		{
			m_CameraPosition += Front * velocity;
		}
		else if (Input::IsKeyPressed(HZ_KEY_S))
		{
			m_CameraPosition -= Front * velocity;
		}

		if (Input::IsKeyPressed(HZ_KEY_LEFT_CONTROL))
		{
			m_CameraPosition -= Up * velocity;
		}
		else if (Input::IsKeyPressed(HZ_KEY_SPACE))
		{
			m_CameraPosition += Up * velocity;
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_CameraTranslationSpeed = m_Camera.m_Fov;
	}

	void PerspectiveCameraController3D::OnEvent(Event& e)
	{
		HZ_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(PerspectiveCameraController3D::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(PerspectiveCameraController3D::OnWindowResized));
		dispatcher.Dispatch<MouseMovedEvent>(HZ_BIND_EVENT_FN(PerspectiveCameraController3D::OnMouseMoved));
	}

	bool PerspectiveCameraController3D::OnMouseScrolled(MouseScrolledEvent& e)
	{
		HZ_PROFILE_FUNCTION();

		if (!Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_RIGHT))
		{
			return false;
		}

		m_Camera.m_Fov -= e.GetYOffset() * 0.1;
		if (m_Camera.m_Fov < 10.0f)
			m_Camera.m_Fov = 10.0f;
		if (m_Camera.m_Fov > 89.0f)
			m_Camera.m_Fov = 89.0f;

		m_Camera.UpdateProjectAndViewMatrix();
		
		return false;
	}


	bool PerspectiveCameraController3D::OnMouseMoved(MouseMovedEvent& e)
	{
		HZ_PROFILE_FUNCTION();

		if (!Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_RIGHT))
		{
			m_isFirstMouse = true;
			return false;
		}

		if (m_isFirstMouse)
		{
			m_LastX = e.GetX();
			m_LastY = e.GetY();
			m_isFirstMouse = false;
		}

		float xoffset = e.GetX() - m_LastX;
		float yoffset = m_LastY - e.GetY();

		m_LastX = e.GetX();
		m_LastY = e.GetY();

		m_Camera.m_Yaw += xoffset * m_MouseSensitivity;
		m_Camera.m_Pitch += yoffset * m_MouseSensitivity;

		if (m_Camera.m_Pitch > 89.0f)
			m_Camera.m_Pitch = 89.0f;
		if (m_Camera.m_Pitch < -89.0f)
			m_Camera.m_Pitch = -89.0f;

		m_Camera.UpdateCameraMoveState();
		m_Camera.RecalculateViewMatrix();

		return false;
	}


	bool PerspectiveCameraController3D::OnWindowResized(WindowResizeEvent& e)
	{
		HZ_PROFILE_FUNCTION();

		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.m_AspectRatio = m_AspectRatio;
		m_Camera.UpdateProjectAndViewMatrix();

		return false;
	}

}