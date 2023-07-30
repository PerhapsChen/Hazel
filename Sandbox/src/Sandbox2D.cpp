#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <glm/glm/gtx/string_cast.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280, 720)
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
		Hazel::Renderer3D::BeginScene(m_CameraController.GetCamera());

		Hazel::Renderer3D::DrawPaimon(m_Position, m_Size, m_LightPos,
							m_CameraController.GetCamera().GetPosition(), m_Blinn, m_Gamma);

		Hazel::Renderer3D::EndScene();
	}

}

void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	ImGui::Begin("Settings");


	ImGui::DragFloat3("Model Position", glm::value_ptr(m_Position), 0.05f);
	ImGui::DragFloat3("Model Scale", glm::value_ptr(m_Size), 0.05f);

	ImGui::Separator();
	
	ImGui::DragFloat3("Light Position", glm::value_ptr(m_LightPos), 0.05f);
	ImGui::Checkbox("Blinn Phong", &m_Blinn);

	ImGui::Separator();

	std::string vecStr = glm::to_string(m_CameraController.GetCamera().GetPosition());
	ImGui::Text("Camera Position: %s", vecStr.c_str());
	ImGui::DragFloat("Gamma", &m_Gamma, 0.01f);


	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}