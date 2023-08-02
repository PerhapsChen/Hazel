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
		static void LoadNewnew();
		static void LoadPlane();
		static void LoadLightCube();

		static void SetActiveScene(int sceneIndex);
		//static void ShadowPrepare();

		// Primitives
		static void DrawLightCube(const glm::vec3& lightPos, const glm::vec3& size, const glm::vec3& color);

		static void FrameBufferMesh(const glm::vec3& position, const glm::vec3& size,
			const glm::vec3& lightPos, const glm::vec3& viewPos, const glm::vec3& lightColor,
			const bool blinn, const float gamma, const float lightIntensity, const float specPow,
			const glm::vec4 orthParas );

		static void DrawMesh(const glm::vec3& position, const glm::vec3& size, 
							 const glm::vec3& lightPos, const glm::vec3& viewPos, const glm::vec3& lightColor,
							 const bool blinn, const float gamma, const float lightIntensity,
							 const float specPow, const int shadowMethod, const glm::vec4 orthParas,
							 const bool autoBias, const float shadowBias, const float outlineScale);

		static void DrawPlane(const glm::vec3& position, const glm::vec3& size,
							const glm::vec3& lightPos, const glm::vec3& viewPos, const glm::vec3& lightColor,
							const bool blinn, const float gamma, const float lightIntensity,
							const float specPow, const int shadowMethod, const glm::vec4 orthParas,
							const bool autoBias, const float shadowBias, const float filterScale);

	};

}