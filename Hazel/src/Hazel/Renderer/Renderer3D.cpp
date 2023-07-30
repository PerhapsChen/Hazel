#include "hzpch.h"
#include "Hazel/Renderer/Renderer3D.h"

#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

// 
#include <tiny_obj_loader.h>

namespace Hazel {

	//struct Renderer3DStorage
	//{
	//	Ref<VertexArray> CubeVertexArray;
	//	Ref<Shader> TextureShader;
	//	Ref<Texture2D> WhiteTexture;
	//};

	struct RendererMeshStorage
	{
		Ref<VertexArray> VertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> DiffTexture;
	};

	//static Renderer3DStorage* s_Data;
	static std::vector<RendererMeshStorage*> s_Data;

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

			s_Data.emplace_back(renderData);
		}
	}

	void Renderer3D::Init()
	{
		HZ_PROFILE_FUNCTION();

		LoadPaimon();




		//Model cube("assets/models/spot/Crate")
	}

	void Renderer3D::Shutdown()
	{
		HZ_PROFILE_FUNCTION();
		for(auto& d: s_Data)
			delete d;
	}

	void Renderer3D::BeginScene(const PerspectiveCamera3D& camera)
	{
		HZ_PROFILE_FUNCTION();
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

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

	}

	void Renderer3D::DrawMesh(const glm::vec3& position, const glm::vec3& size)
	{
		HZ_PROFILE_FUNCTION();

	}

	void Renderer3D::DrawPaimon(const glm::vec3& position, const glm::vec3& size, 
							    const glm::vec3& lightPos, const glm::vec3& viewPos,
								const bool blinn, const float gamma)
	{
		HZ_PROFILE_FUNCTION();

		for (auto& d : s_Data)
		{
			//HZ_CORE_TRACE("{good.}");
			//d->TextureShader->SetFloat4("u_Color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			//d->TextureShader->SetFloat("u_TilingFactor", 1.0f);
			d->DiffTexture->Bind();

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });

			d->TextureShader->SetMat4("u_Transform", transform);
			d->TextureShader->SetFloat3("lightPos", lightPos);
			d->TextureShader->SetFloat3("viewPos", viewPos );
			d->TextureShader->SetInt("blinn", blinn);
			d->TextureShader->SetFloat("gamma", gamma);
			d->VertexArray->Bind();
			RenderCommand::DrawIndexed(d->VertexArray);
		}

	}

}