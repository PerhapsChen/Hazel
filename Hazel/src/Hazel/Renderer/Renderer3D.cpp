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

	//struct ShadowProps
	//{
	//	uint32_t SHADOW_WIDTH;
	//	uint32_t SHADOW_HEIGHT;
	//	std::string shadowMethod;
	//};

	struct RendererMeshStorage
	{
		Ref<VertexArray> VertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> DiffTexture;
	};

	static Ref<Shader> s_OnlyBackShader;
	static Ref<Shader> s_LightViewDepthShader;
	static Ref<FrameBuffer> s_DepthFrameBuffer;
	
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
			renderData->TextureShader = Shader::Create("assets/shaders/LightingWithShadow_Mesh.glsl");
			renderData->TextureShader->Bind();
			renderData->DiffTexture->Bind(0);
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
			renderData->TextureShader = Shader::Create("assets/shaders/LightingWithShadow_Mesh.glsl");
			renderData->TextureShader->Bind();
			renderData->DiffTexture->Bind(0);
			renderData->TextureShader->SetInt("diffuseTexture", 0);
			renderData->TextureShader->Unbind();

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
		s_DataPlane->TextureShader = Shader::Create("assets/shaders/LightingWithShadow_Plane.glsl");
		s_DataPlane->TextureShader->Bind();
		s_DataPlane->DiffTexture->Bind(0);
		s_DataPlane->TextureShader->SetInt("diffuseTexture", 0);
		s_DataPlane->TextureShader->Unbind();
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
		LoadLightCube();
		LoadPlane();
		LoadNewnew();
		LoadPaimon();

		s_OnlyBackShader = Shader::Create("assets/shaders/OnlyBack.glsl");

		s_LightViewDepthShader = Shader::Create("assets/shaders/LightViewDepth.glsl");
		s_DepthFrameBuffer = FrameBuffer::Create(4096, 4096);

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
		// 只是给Shader传参

		HZ_PROFILE_FUNCTION();

		s_DataPlane->TextureShader->Bind(); //！
		s_DataPlane->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_DataLightCube->TextureShader->Bind(); //！
		s_DataLightCube->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		auto& s_Data = *s_ActvieScene;
		for (auto& d : s_Data)
		{
			d->TextureShader->Bind(); //！
			d->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		}

		s_OnlyBackShader->Bind();
		s_OnlyBackShader->SetMat4("u_View", camera.GetViewMatrix());
		s_OnlyBackShader->SetMat4("u_Projection", camera.GetProjectionMatrix());
	}

	void Renderer3D::EndScene()
	{
		HZ_PROFILE_FUNCTION();

	}

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

	//! Frame Buffer
	void Renderer3D::FrameBufferMesh(const glm::vec3& position, const glm::vec3& size,
		const glm::vec3& lightPos, const glm::vec3& viewPos, const glm::vec3& lightColor,
		const bool blinn, const float gamma, const float lightIntensity, const float specPow, 
		const glm::vec4 orthParas)
	{
		HZ_PROFILE_FUNCTION();
		auto& s_Data = *s_ActvieScene;

		glm::mat4 lightProjection = glm::ortho(orthParas.r, -orthParas.r, orthParas.g, -orthParas.g, orthParas.b, orthParas.a);
		glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		s_LightViewDepthShader->Bind();
		s_LightViewDepthShader->SetMat4("transform", transform);
		s_LightViewDepthShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

		s_DepthFrameBuffer->Bind();

		for (auto& d : s_Data)
		{
			d->VertexArray->Bind();
			RenderCommand::DrawIndexed(d->VertexArray); // 这里会解除绑定texture
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, 1920, 1080);
	}

	void Renderer3D::DrawMesh(const glm::vec3& position, const glm::vec3& size,
		const glm::vec3& lightPos, const glm::vec3& viewPos, const glm::vec3& lightColor,
		const bool blinn, const float gamma, const float lightIntensity,
		const float specPow, const int shadowMethod, const glm::vec4 orthParas,
		const bool autoBias, const float shadowBias, const float outlineScale)
	{
		HZ_PROFILE_FUNCTION();
		auto& s_Data = *s_ActvieScene;

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		// Back Only Pass
		s_OnlyBackShader->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
			glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_OnlyBackShader->SetMat4("u_Transform", transform);
		s_OnlyBackShader->SetFloat3("lightPos", lightPos);
		s_OnlyBackShader->SetFloat3("lightColor", lightColor);
		s_OnlyBackShader->SetFloat3("viewPos", viewPos);
		s_OnlyBackShader->SetFloat("gamma", gamma);
		s_OnlyBackShader->SetFloat("outlineScale", outlineScale);

		for (auto& d : s_Data)
		{
			d->VertexArray->Bind();
			d->DiffTexture->Bind();
			s_OnlyBackShader->SetInt("diffuseTexture", 0);
			RenderCommand::DrawIndexed(d->VertexArray);
		}

		// Enf Of BackLine;

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glm::mat4 lightProjection = glm::ortho(orthParas.r, -orthParas.r, orthParas.g, -orthParas.g, orthParas.b, orthParas.a);
		glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		for (auto& d : s_Data)
		{
			d->TextureShader->Bind(); // use shader

			d->DiffTexture->Bind(0); 

			s_DepthFrameBuffer->GetDepthAttachmentTexture()->Bind(1);

			d->TextureShader->SetInt("shadowMap", 1);

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
			
			d->TextureShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
			
			d->TextureShader->SetMat4("u_Transform", transform);
			d->TextureShader->SetFloat3("lightPos", lightPos);
			d->TextureShader->SetFloat3("lightColor", lightColor);
			d->TextureShader->SetFloat3("viewPos", viewPos);
			d->TextureShader->SetInt("blinn", blinn);
			d->TextureShader->SetFloat("gamma", gamma);
			d->TextureShader->SetFloat("specPow", specPow);
			d->TextureShader->SetFloat("lightIntensity", lightIntensity);
			d->TextureShader->SetInt("shadowMethod", shadowMethod);

			d->TextureShader->SetInt("autoBias", autoBias);
			d->TextureShader->SetFloat("shadowBias", shadowBias);

			d->VertexArray->Bind();

			RenderCommand::DrawIndexed(d->VertexArray);
		}

		glDisable(GL_CULL_FACE);
	}

	void Renderer3D::DrawPlane(const glm::vec3& position, const glm::vec3& size,
							   const glm::vec3& lightPos, const glm::vec3& viewPos, const glm::vec3& lightColor,
							   const bool blinn, const float gamma, const float lightIntensity,
							   const float specPow, const int shadowMethod, const glm::vec4 orthParas,
							   const bool autoBias, const float shadowBias, const float filterScale)
	{
		HZ_PROFILE_FUNCTION();

		glm::mat4 lightProjection = glm::ortho(orthParas.r, -orthParas.r, orthParas.g, -orthParas.g, orthParas.b, orthParas.a);
		glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		s_DataPlane->TextureShader->Bind();
		s_DataPlane->DiffTexture->Bind(0);

		s_DepthFrameBuffer->GetDepthAttachmentTexture()->Bind(1);

		s_DataPlane->TextureShader->SetInt("shadowMap", 1);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_DataPlane->TextureShader->SetMat4("u_Transform", transform);

		s_DataPlane->TextureShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

		s_DataPlane->TextureShader->SetFloat3("lightPos", lightPos);
		s_DataPlane->TextureShader->SetFloat3("lightColor", lightColor);
		s_DataPlane->TextureShader->SetFloat3("viewPos", viewPos);
		s_DataPlane->TextureShader->SetInt("blinn", blinn);
		s_DataPlane->TextureShader->SetFloat("gamma", gamma);
		s_DataPlane->TextureShader->SetFloat("specPow", specPow);
		s_DataPlane->TextureShader->SetFloat("lightIntensity", lightIntensity);
		s_DataPlane->TextureShader->SetInt("shadowMethod", shadowMethod);

		s_DataPlane->TextureShader->SetInt("autoBias", autoBias);
		s_DataPlane->TextureShader->SetFloat("shadowBias", shadowBias);

		s_DataPlane->TextureShader->SetFloat("filterScale", filterScale);

		s_DataPlane->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_DataPlane->VertexArray);
	}




}