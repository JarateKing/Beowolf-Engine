#include "BMWModel.h"
#include <iostream>

namespace wolf
{
	BMWModel::BMWModel(std::string file, std::string vertexShader, std::string pixelShader) {
		BMWLoader::getInstance().loadFile(file, &m_textures, &m_meshes, &m_rootNode);
		std::cout << m_textures.size() << " " << m_meshes.size() << " " << m_rootNode.children.size() << "\n";
	}
}