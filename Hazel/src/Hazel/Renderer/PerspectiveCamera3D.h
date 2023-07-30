#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	class PerspectiveCamera3D
	{
		friend class PerspectiveCameraController3D;
	public:
		//PerspectiveCamera3D(float verticalFVO, float nearClip, float farClip); 

		PerspectiveCamera3D(float fov, float aspect_ratio, float znear, float zfar);

		void SetProjection(float fov, float aspect_ratio, float znear, float zfar);

		const glm::vec3& GetPosition() const { return m_Position; }

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; } // 投影矩阵 最终的。
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; } // 相机(人眼)为原点的
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; } // x' = PV(M)x

	private:
		void RecalculateViewMatrix();

		void UpdateProjectAndViewMatrix();
		
		void UpdateCameraMoveState();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_WorldUp;
		glm::vec3 m_Right;
		glm::vec3 m_Up;

		float m_Yaw;
		float m_Pitch;

		float m_Fov;
		float m_ZNear;
		float m_ZFar;
		float m_AspectRatio;
	};

}