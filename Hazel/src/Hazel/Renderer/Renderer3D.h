#pragma once

#include "Hazel.h"

#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Renderer/PerspectiveCamera3D.h"

#include "Hazel/Renderer/Texture.h"

namespace Hazel {

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const PerspectiveCamera3D& camera);
		static void EndScene();

		// load in Init()
		static void LoadPaimon();

		// Primitives
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawMesh(const glm::vec3& position, const glm::vec3& size);

		static void DrawPaimon(const glm::vec3& position, const glm::vec3& size,
							   const glm::vec3& lightPos, const glm::vec3& viewPos,
							   const bool blinn, const float gamma);

	};

}