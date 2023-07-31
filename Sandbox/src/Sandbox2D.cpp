#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <glm/glm/gtx/string_cast.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1920, 1080)
{
}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();
	ImGui::GetIO().Fonts->AddFontFromFileTTF("assets/fonts/Cousine-Regular.ttf", 18.0f);

	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	HZ_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION();
	// Update
	m_CameraController.OnUpdate(ts);
	// Render
	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();
	}

	{
		HZ_PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer3D::SetActiveScene(m_Scene);
		Hazel::Renderer3D::BeginScene(m_CameraController.GetCamera());

		Hazel::Renderer3D::DrawLightCube(m_LightPos, m_LightSize, m_LightColor);

		Hazel::Renderer3D::DrawPlane(m_PlanePosition, m_PlaneSize, glm::vec4(m_PlaneColor, 1.0f), 
			m_LightPos,  m_CameraController.GetCamera().GetPosition(), m_LightColor, 
			m_Blinn, m_Gamma, m_LightIntensity, m_SpecPow);

		Hazel::Renderer3D::DrawMesh(m_Position, m_Size, m_LightPos, 
			m_CameraController.GetCamera().GetPosition(), m_LightColor, 
			m_Blinn, m_Gamma, m_LightIntensity, m_SpecPow);

		Hazel::Renderer3D::EndScene();
	}

}

void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	ImGui::DragInt("Scene", &m_Scene, 1, 1, 2);

	ImGui::DragFloat3("Model Position", glm::value_ptr(m_Position), 0.05f);
	ImGui::DragFloat3("Model Scale", glm::value_ptr(m_Size), 0.05f);

	ImGui::Separator();
	ImGui::DragFloat3("Plane Position", glm::value_ptr(m_PlanePosition), 0.05f);
	ImGui::DragFloat3("Plane Scale", glm::value_ptr(m_PlaneSize), 0.05f);
	
	ImGui::Separator();
	
	ImGui::DragFloat3("Light Position", glm::value_ptr(m_LightPos), 0.05f);
	ImGui::ColorEdit3("Light Color", glm::value_ptr(m_LightColor));
	ImGui::DragFloat("Light Intensity", &m_LightIntensity, 0.05f, 0.0f, 5.0f);
	ImGui::DragFloat("Specular Pow", &m_SpecPow, 1.0f, 1.0f, 256.0f);
	ImGui::Checkbox("Blinn Phong", &m_Blinn);

	ImGui::Separator();

	std::string vecStr = glm::to_string(m_CameraController.GetCamera().GetPosition());
	ImGui::Text("Camera Position:\n %s", vecStr.c_str());
	ImGui::DragFloat("Gamma", &m_Gamma, 0.01f);


	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}