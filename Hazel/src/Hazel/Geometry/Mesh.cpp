#include "hzpch.h"

#include "Mesh.h"

namespace Hazel
{
    Mesh::Mesh(aiMesh* mesh, const aiScene* scene, std::string directory):
        m_Mesh(mesh), m_Scene(scene), m_Directory(directory)
    {
        m_Material = m_Scene->mMaterials[mesh->mMaterialIndex];
        //m_TexturePath = m_Material.
        InitVerticesAndIdices();
        InitTexturePath();
    }

    void Mesh::InitVerticesAndIdices()
    {
        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < m_Mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;
            // positions
            vector.x = m_Mesh->mVertices[i].x;
            vector.y = m_Mesh->mVertices[i].y;
            vector.z = m_Mesh->mVertices[i].z;
            vertex.position = vector;
            // normals
            if (m_Mesh->HasNormals())
            {
                vector.x = m_Mesh->mNormals[i].x;
                vector.y = m_Mesh->mNormals[i].y;
                vector.z = m_Mesh->mNormals[i].z;
                vertex.normal = vector;
            }
            // texture coordinates
            if (m_Mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                vec.x = m_Mesh->mTextureCoords[0][i].x;
                vec.y = m_Mesh->mTextureCoords[0][i].y;
                vertex.texCoord = vec;
            }
            else
                vertex.texCoord = glm::vec2(0.0f, 0.0f);

            m_Vertices.push_back(vertex);
        }
        // indices.
        for (unsigned int i = 0; i < m_Mesh->mNumFaces; i++)
        {
            aiFace face = m_Mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                m_Indices.push_back(face.mIndices[j]);
        }
    }

    void Mesh::InitTexturePath()
    {
        aiString str;
        m_Material->GetTexture(aiTextureType_DIFFUSE, 0, &str); 
        m_TexturePath = m_Directory + '/' + std::string(str.C_Str());

        std::cout << m_TexturePath << std::endl;
    }

    float* Mesh::GetVertexBuffers()
    {
        m_Attri.reserve(m_Vertices.size() * 8);
        for (auto& v : m_Vertices)
        {
            m_Attri.emplace_back(v.position.x);
            m_Attri.emplace_back(v.position.y);
            m_Attri.emplace_back(v.position.z);
            m_Attri.emplace_back(v.normal.x);
            m_Attri.emplace_back(v.normal.y);
            m_Attri.emplace_back(v.normal.z);
            m_Attri.emplace_back(v.texCoord.x);
            m_Attri.emplace_back(v.texCoord.y);
        }
        return m_Attri.data();
    }

    uint32_t* Mesh::GetIndicesBuffers()
    {
        return m_Indices.data();
    }

    uint32_t Mesh::VertexBufferSize()
    {
        return m_Attri.size();
    }
    uint32_t Mesh::IndicesBufferSize()
    {
        return m_Indices.size();
    }

    std::string Mesh::GetTexturePath()
    {
        return m_TexturePath;
    }

}