#pragma once

#include "Hazel/Renderer/PerspectiveCamera3D.h"
#include "Hazel/Core/Timestep.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel {

	class PerspectiveCameraController3D
	{
	public:
		PerspectiveCameraController3D(uint32_t viewport_width, uint32_t viewport_height);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		PerspectiveCamera3D& GetCamera() { return m_Camera; }
		const PerspectiveCamera3D& GetCamera() const { return m_Camera; }

		//float GetZoomLevel() const { return m_Camera.m_Zoom; }
		//void SetZoomLevel(float level) { m_Camera.m_Zoom = level; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);
	private:
		float m_AspectRatio;

		PerspectiveCamera3D m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };

		float m_CameraTranslationSpeed = 1e-5f;
		float m_MouseSensitivity = 0.1f;

		float m_LastX = 0.0f;
		float m_LastY = 0.0f;
		float m_isFirstMouse = false;

		uint32_t m_ViewPortWidth = 0;
		uint32_t m_ViewPortHeight = 0;

	};

}