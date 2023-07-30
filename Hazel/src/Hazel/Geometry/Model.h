#pragma once 

//#include "Hazel.h"
#include "Mesh.h"

namespace Hazel
{
	class Model 
	{
	public:
		Model() = delete;
		Model(const std::string& path);

		virtual ~Model() = default;

	private:
		void processNode(aiNode* node, const aiScene* scene);

	public:
		std::vector<Mesh> m_Meshes;
		std::string m_Directory;
		//bool m_GammaCorrection;
	};
}
