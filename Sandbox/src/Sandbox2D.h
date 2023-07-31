#pragma once

#include "Hazel.h"

class Sandbox2D : public Hazel::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;
private:
	//Hazel::OrthographicCameraController m_CameraController;
	Hazel::PerspectiveCameraController3D m_CameraController;

	// Temp
	//Hazel::Ref<Hazel::VertexArray> m_SquareVA;
	//Hazel::Ref<Hazel::Shader> m_FlatColorShader;

	Hazel::Ref<Hazel::Texture2D> m_CheckerboardTexture;

	int m_Scene = 1;
	// model
	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_Size = { 5.0f, 5.0f, 5.0f };

	// plane
	glm::vec3 m_PlanePosition = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_PlaneColor = { 1.0f, 1.0f, 1.0f};
	glm::vec3 m_PlaneSize = { 10.0f, 1.0f, 10.0f };

	// camera

	// light 
	glm::vec3 m_LightPos = { 1.3f, 4.5f, 4.6f };
	glm::vec3 m_LightSize = { 0.2f, 0.2f, 0.2f };
	glm::vec3 m_LightColor = { 1.0f, 1.0f, 1.0f };
	

	// shader
	bool m_Blinn = true;
	float m_Gamma = 2.2;
	float m_SpecPow = 32.0f;
	float m_LightIntensity = 1.0f;

};