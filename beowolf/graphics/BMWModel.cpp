#include "BMWModel.h"
#include <iostream>
#include <stack>
#include "W_BufferManager.h"

namespace wolf
{
	BMWModel::BMWModel(std::string file, std::string vertexShader, std::string pixelShader)
	{
		BMWLoader::getInstance().loadFile(file, &m_textures, &m_vertices, &m_indices, &m_rootNode);

		// set up m_meshes
		for (int i = 0; i < m_vertices.size(); i++) {
			Mesh current;
			current.size = m_vertices[i].size();
			current.m_pVB = wolf::BufferManager::CreateVertexBuffer(&(m_vertices[i][0]), sizeof(Vertex) * m_vertices[i].size());
			current.m_pProg = wolf::ProgramManager::CreateProgram(vertexShader, pixelShader);

			current.m_pIB = wolf::BufferManager::CreateIndexBuffer(m_indices[i].size() * 3);
			current.m_pIB->Write(&m_indices[i], m_indices[i].size() * 3);

			current.m_pDecl = new wolf::VertexDeclaration();
			current.m_pDecl->Begin();
			current.m_pDecl->AppendAttribute(AT_Position, 3, CT_Float);
			current.m_pDecl->AppendAttribute(AT_Color, 4, CT_UByte);
			current.m_pDecl->AppendAttribute(AT_TexCoord1, 2, CT_Float);
			current.m_pDecl->AppendAttribute(AT_Normal, 3, CT_Float);
			current.m_pDecl->SetVertexBuffer(current.m_pVB);
			current.m_pDecl->SetIndexBuffer(current.m_pIB);
			current.m_pDecl->End();

			m_meshes.push_back(current);
		}

		std::stack<BMWNode> nodes;
		nodes.push(m_rootNode);
		while (!nodes.empty()) {
			BMWNode current = nodes.top();
			nodes.pop();
			for (int i = 0; i < current.children.size(); i++)
				nodes.push(current.children[i]);

			for (int i = 0; i < current.meshNum; i++) {
				NodeMesh renderable;
				renderable.transform = current.transform;
				renderable.meshID = current.meshIDs[i];
				m_toRender.push_back(renderable);
			}
		}
	}

	void BMWModel::update(float delta)
	{

	}

	void BMWModel::render(glm::mat4 view, glm::mat4 proj)
	{
		for (int i = 0; i < m_toRender.size(); i++) {
			m_meshes[m_toRender[i].meshID].m_pDecl->Bind();

			m_meshes[m_toRender[i].meshID].m_pProg->Bind();
			m_meshes[m_toRender[i].meshID].m_pProg->SetUniform("projection", proj);
			m_meshes[m_toRender[i].meshID].m_pProg->SetUniform("view", view);
			m_meshes[m_toRender[i].meshID].m_pProg->SetUniform("world", glm::mat4());
			
			// Draw!
			glDrawElements(GL_TRIANGLES, m_meshes[m_toRender[i].meshID].m_pIB->GetNumIndices(), GL_UNSIGNED_SHORT, 0);
		}
	}
}