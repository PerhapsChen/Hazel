#include "hzpch.h"
#include "Hazel/Renderer/Renderer3D.h"

#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

// 
#include <glad/glad.h>

namespace Hazel {

	//struct Renderer3DStorage
	//{
	//	Ref<VertexArray> CubeVertexArray;
	//	Ref<Shader> TextureShader;
	//	Ref<Texture2D> WhiteTexture;
	//};

	struct ShadowProps
	{
		uint32_t SHADOW_WIDTH;
		uint32_t SHADOW_HEIGHT;
		std::string shadowMethod;
	};

	struct RendererMeshStorage
	{
		Ref<VertexArray> VertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> DiffTexture;
	};

	//static Renderer3DStorage* s_Data;
	static std::vector<RendererMeshStorage*> s_DataPaimon;
	static std::vector<RendererMeshStorage*> s_DataNewnew;	

	static std::vector<RendererMeshStorage*>* s_ActvieScene = &s_DataPaimon;

	void Renderer3D::SetActiveScene(int sceneIndex)
	{
		switch (sceneIndex)
		{
		case 1:
			s_ActvieScene = &s_DataNewnew;
			break;
		
		case 2:
			s_ActvieScene = &s_DataPaimon;
			break;

		default:
			break;
		}
	}


	void Renderer3D::LoadNewnew()
	{
		Model newnew("assets/models/newnew/newnew.obj");
		for (auto& mesh : newnew.m_Meshes)
		{
			//std::vector<RendererMeshStorage*> s_Data;

			RendererMeshStorage* renderData = new RendererMeshStorage();

			auto texPath = mesh.GetTexturePath();
			auto vertexBuffers = mesh.GetVertexBuffers();
			auto indicesBuffers = mesh.GetIndicesBuffers();

			renderData->VertexArray = VertexArray::Create();
			Ref<VertexBuffer> VB = VertexBuffer::Create(vertexBuffers, mesh.VertexBufferSize() * sizeof(float));
			VB->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Normal" },
				{ ShaderDataType::Float2, "a_TexCoord" } });

			renderData->VertexArray->AddVertexBuffer(VB);

			Ref<IndexBuffer> IB = IndexBuffer::Create(indicesBuffers, mesh.IndicesBufferSize());
			renderData->VertexArray->SetIndexBuffer(IB);

			int flipV = 0;
			renderData->DiffTexture = Texture2D::Create(texPath, flipV);
			HZ_CORE_TRACE(texPath);
			renderData->TextureShader = Shader::Create("assets/shaders/AdvancedLighting.glsl");
			renderData->TextureShader->Bind();
			renderData->TextureShader->SetInt("diffuseTexture", 0);

			HZ_CORE_TRACE("{0}", mesh.VertexBufferSize());

			s_DataNewnew.emplace_back(renderData);
		}
	}

	void Renderer3D::LoadPaimon()
	{
		Model paimon("assets/models/paimon/paimon.obj");
		for (auto& mesh : paimon.m_Meshes)
		{
			//std::vector<RendererMeshStorage*> s_Data;

			RendererMeshStorage* renderData = new RendererMeshStorage();

			auto texPath = mesh.GetTexturePath();
			auto vertexBuffers = mesh.GetVertexBuffers();
			auto indicesBuffers = mesh.GetIndicesBuffers();

			renderData->VertexArray = VertexArray::Create();
			Ref<VertexBuffer> VB = VertexBuffer::Create(vertexBuffers, mesh.VertexBufferSize() * sizeof(float));
			VB->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Normal" },
				{ ShaderDataType::Float2, "a_TexCoord" } });

			renderData->VertexArray->AddVertexBuffer(VB);

			Ref<IndexBuffer> IB = IndexBuffer::Create(indicesBuffers, mesh.IndicesBufferSize());
			renderData->VertexArray->SetIndexBuffer(IB);

			int flipV = 0;
			renderData->DiffTexture = Texture2D::Create(texPath, flipV);
			HZ_CORE_TRACE(texPath);
			renderData->TextureShader = Shader::Create("assets/shaders/AdvancedLighting.glsl");
			renderData->TextureShader->Bind();
			renderData->TextureShader->SetInt("diffuseTexture", 0);

			HZ_CORE_TRACE("{0}", mesh.VertexBufferSize());

			s_DataPaimon.emplace_back(renderData);
		}
	}

	static RendererMeshStorage* s_DataPlane;

	void Renderer3D::LoadPlane()
	{
		s_DataPlane = new RendererMeshStorage();
		s_DataPlane->VertexArray = VertexArray::Create();
		float planeVertices[8 * 4] = {
			-0.5f, 0.0f, -0.5f, 0.0f, 1.0f ,0.0f, 0.0f, 0.0f,
			 0.5f, 0.0f, -0.5f, 0.0f, 1.0f ,0.0f, 1.0f, 0.0f,
			 0.5f, 0.0f,  0.5f, 0.0f, 1.0f ,0.0f, 1.0f, 1.0f,
			-0.5f, 0.0f,  0.5f, 0.0f, 1.0f ,0.0f, 0.0f, 1.0f
		};
		Ref<VertexBuffer> VB = VertexBuffer::Create(planeVertices, sizeof(planeVertices));
		VB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_DataPlane->VertexArray->AddVertexBuffer(VB);

		uint32_t planeIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> IB = IndexBuffer::Create(planeIndices, sizeof(planeIndices) / sizeof(uint32_t));
		s_DataPlane->VertexArray->SetIndexBuffer(IB);

		// create Texture ( flat color )
		s_DataPlane->DiffTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_DataPlane->DiffTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		// create Shader uniform texture
		s_DataPlane->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_DataPlane->TextureShader->Bind();
		s_DataPlane->TextureShader->SetInt("u_Texture", 0);
	}

	static RendererMeshStorage* s_DataLightCube;

	void Renderer3D::LoadLightCube()
	{
		s_DataLightCube = new RendererMeshStorage();
		s_DataLightCube->VertexArray = VertexArray::Create();
		float lightCubeVertices[36 * 8] = {
			// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
		};
		Ref<VertexBuffer> VB = VertexBuffer::Create(lightCubeVertices, sizeof(lightCubeVertices));
		VB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_DataLightCube->VertexArray->AddVertexBuffer(VB);

		uint32_t lightCubeIndices[36] = { 
			 0,  1,  2,  3,  4,  5, 
			 6,  7,  8,  9, 10, 11, 
			12, 13, 14, 15, 16, 17, 
			18, 19, 20, 21, 22, 23,
			24, 25, 26, 27, 28, 29,
			30, 31, 32, 33, 34, 35};

		Ref<IndexBuffer> IB = IndexBuffer::Create(lightCubeIndices, sizeof(lightCubeIndices) / sizeof(uint32_t));
		s_DataLightCube->VertexArray->SetIndexBuffer(IB);

		// create Texture ( flat color )
		s_DataLightCube->DiffTexture = Texture2D::Create(1, 1);

		// create Shader uniform texture
		s_DataLightCube->TextureShader = Shader::Create("assets/shaders/LightCube.glsl");
		s_DataLightCube->TextureShader->Bind();
	}

	void Renderer3D::Init()
	{
		HZ_PROFILE_FUNCTION();

		//? SHADOW
		//const GLuint SHADOW_WIDTH = 8192, SHADOW_HEIGHT = 8192;
		//GLuint DepthFrameBuffer;
		//glGenFramebuffers(1, &DepthFrameBuffer);

		//GLuint DepthTexture;
		//glGenTextures(1, &DepthTexture);
		//glBindTexture(GL_TEXTURE_2D, DepthTexture);

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 
		//	0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		//glBindFramebuffer(GL_FRAMEBUFFER, DepthFrameBuffer);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthTexture, 0);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//? SHADOW


		LoadLightCube();
		LoadPlane();
		LoadNewnew();
		LoadPaimon();
		
		//Model cube("assets/models/spot/Crate")
	}

	void Renderer3D::Shutdown()
	{
		auto& s_Data = *s_ActvieScene;
		delete s_DataPlane;
		delete s_DataLightCube;

		HZ_PROFILE_FUNCTION();
		for(auto& d: s_Data)
			delete d;
	}

	void Renderer3D::BeginScene(const PerspectiveCamera3D& camera)
	{
		HZ_PROFILE_FUNCTION();

		s_DataPlane->TextureShader->Bind(); //£¡
		s_DataPlane->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_DataLightCube->TextureShader->Bind(); //£¡
		s_DataLightCube->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		auto& s_Data = *s_ActvieScene;
		for (auto& d : s_Data)
		{
			d->TextureShader->Bind(); //£¡
			d->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		}

	}

	void Renderer3D::EndScene()
	{
		HZ_PROFILE_FUNCTION();

	}

	//void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	//{
	//	HZ_PROFILE_FUNCTION();

	//}


	void Renderer3D::DrawLightCube(const glm::vec3& lightPos, const glm::vec3& size, const glm::vec3& color)
	{
		HZ_PROFILE_FUNCTION();

		s_DataLightCube->TextureShader->Bind();
		s_DataLightCube->TextureShader->SetFloat3("u_Color", color);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), lightPos) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_DataLightCube->TextureShader->SetMat4("u_Transform", transform);
		s_DataLightCube->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_DataLightCube->VertexArray);
	}

	void Renderer3D::DrawPlane(const glm::vec3 position, const glm::vec3& size, const glm::vec4& color,
							   const glm::vec3& lightPos, const glm::vec3& viewPos, const glm::vec3& lightColor,
							   const bool blinn, const float gamma, const float lightIntensity,
							   const float specPow)
	{
		HZ_PROFILE_FUNCTION();

		s_DataPlane->TextureShader->Bind();
		s_DataPlane->DiffTexture->Bind();
		s_DataPlane->TextureShader->SetFloat4("u_Color", color);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_DataPlane->TextureShader->SetMat4("u_Transform", transform);

		s_DataPlane->TextureShader->SetFloat3("lightPos", lightPos);
		s_DataPlane->TextureShader->SetFloat3("lightColor", lightColor);
		s_DataPlane->TextureShader->SetFloat3("viewPos", viewPos);
		s_DataPlane->TextureShader->SetInt("blinn", blinn);
		s_DataPlane->TextureShader->SetFloat("gamma", gamma);
		s_DataPlane->TextureShader->SetFloat("specPow", specPow);
		s_DataPlane->TextureShader->SetFloat("lightIntensity", lightIntensity);

		s_DataPlane->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_DataPlane->VertexArray);
	}


	void Renderer3D::DrawMesh(const glm::vec3& position, const glm::vec3& size, 
		const glm::vec3& lightPos, const glm::vec3& viewPos, const glm::vec3& lightColor,
		const bool blinn, const float gamma, const float lightIntensity,
		const float specPow)
	{
		HZ_PROFILE_FUNCTION();
		auto& s_Data = *s_ActvieScene;

		for (auto& d : s_Data)
		{
			//HZ_CORE_TRACE("{good.}");
			//d->TextureShader->SetFloat4("u_Color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			//d->TextureShader->SetFloat("u_TilingFactor", 1.0f);
			d->TextureShader->Bind();
			d->DiffTexture->Bind();

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });

			d->TextureShader->SetMat4("u_Transform", transform);
			d->TextureShader->SetFloat3("lightPos", lightPos);
			d->TextureShader->SetFloat3("lightColor", lightColor);
			d->TextureShader->SetFloat3("viewPos", viewPos );
			d->TextureShader->SetInt("blinn", blinn);
			d->TextureShader->SetFloat("gamma", gamma);
			d->TextureShader->SetFloat("specPow", specPow);
			d->TextureShader->SetFloat("lightIntensity", lightIntensity);
			d->VertexArray->Bind();
			RenderCommand::DrawIndexed(d->VertexArray);
		}

	}

}