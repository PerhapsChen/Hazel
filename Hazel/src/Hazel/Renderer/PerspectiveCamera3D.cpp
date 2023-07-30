#include "hzpch.h"
#include "PerspectiveCamera3D.h"

namespace Hazel {

	PerspectiveCamera3D::PerspectiveCamera3D(float fov, float aspect_ratio, float znear, float zfar):
		m_Position(glm::vec3{ 0.0f,0.0f,0.0f }), m_WorldUp(glm::vec3{ 0.0f,1.0f,0.0f }), m_Front(glm::vec3{ 0.0f,0.0f,-1.0f }),
		m_Fov(fov), m_ZNear(znear), m_ZFar(zfar), m_AspectRatio(aspect_ratio),
		m_Yaw(-180.0f), m_Pitch(0.0f)
	{
		HZ_PROFILE_FUNCTION();
		UpdateCameraMoveState();
		UpdateProjectAndViewMatrix();
	}


	void PerspectiveCamera3D::SetProjection(float fov, float aspect_ratio, float znear, float zfar)
	{
		HZ_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::perspective(fov, aspect_ratio, znear, zfar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera3D::RecalculateViewMatrix()
	{
		HZ_PROFILE_FUNCTION();

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera3D::UpdateProjectAndViewMatrix()
	{
		HZ_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::perspective(m_Fov, m_AspectRatio, m_ZNear, m_ZFar);
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera3D::UpdateCameraMoveState()
	{
		m_Front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front.y = sin(glm::radians(m_Pitch));
		m_Front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(m_Front);
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	}

}

