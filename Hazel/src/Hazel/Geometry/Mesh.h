#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

namespace Hazel
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

	class Mesh
	{
	public:
		Mesh(aiMesh* mesh, const aiScene* scene, std::string directory);
		virtual ~Mesh() = default;

		float* GetVertexBuffers();
		uint32_t* GetIndicesBuffers();

		uint32_t VertexBufferSize();
		uint32_t IndicesBufferSize();

		std::string GetTexturePath();

	private: 
		void InitVerticesAndIdices();
		void InitTexturePath();


	private:
		aiMesh* m_Mesh;
		const aiScene* m_Scene;
		aiMaterial* m_Material;
		std::vector<Vertex>   m_Vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<float> m_Attri;
		std::string m_MeshName;
		std::string m_Directory;
		std::string m_TexturePath;
	};
}

