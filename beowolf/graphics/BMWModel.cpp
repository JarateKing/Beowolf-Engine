#include "BMWModel.h"
#include <iostream>
#include <stack>
#include "W_BufferManager.h"

namespace wolf
{
	BMWModel::BMWModel(std::string file, std::string vertexShader, std::string pixelShader)
	{
		BMWLoader::getInstance().loadFile(file, &m_textures, &m_vertices, &m_indices, &m_rootNode, &m_nodeIDs, &m_boneWeights, &m_anims);

		// set up m_meshes
		for (int i = 0; i < m_vertices.size(); i++) {
			Mesh current;

			current.size = m_vertices[i].size();
			current.m_pVB = wolf::BufferManager::CreateVertexBuffer(&(m_vertices[i][0]), sizeof(Vertex) * m_vertices[i].size());

			current.m_pIB = wolf::BufferManager::CreateIndexBuffer(m_indices[i].size());
			current.m_pIB->Write(&(m_indices[i][0]), m_indices[i].size() * sizeof(unsigned int));

			current.m_pProg = wolf::ProgramManager::CreateProgram(vertexShader, pixelShader);

			current.m_pTex = NULL;
			if (m_textures.size() > i) {
				bool transparency = false; // TODO: improve, naive assumption that jpg's always mean opaque and png's always mean transparent
				if (m_textures[i].find(".jpg") != std::string::npos)
					m_textures[i] = m_textures[i].substr(0, m_textures[i].find(".jpg")) + std::string(".dds");
				else if (m_textures[i].find(".png") != std::string::npos) {
					m_textures[i] = m_textures[i].substr(0, m_textures[i].find(".png")) + std::string(".dds");
					transparency = true;
				}
				m_textures[i] = "resources/models/" + m_textures[i];

				current.m_pTex = wolf::TextureManager::CreateTexture(m_textures[i]);
				current.m_pTex->SetWrapMode(wolf::Texture::WM_Repeat);
				current.isTransparent = transparency;
			}

			current.m_pDecl = new wolf::VertexDeclaration();
			current.m_pDecl->Begin();
			current.m_pDecl->AppendAttribute(AT_Position, 3, CT_Float);
			current.m_pDecl->AppendAttribute(AT_Color, 4, CT_UByte);
			current.m_pDecl->AppendAttribute(AT_TexCoord1, 2, CT_Float);
			current.m_pDecl->AppendAttribute(AT_Normal, 3, CT_Float);
			current.m_pDecl->AppendAttribute(AT_BoneIndices, 4, CT_UInt);
			current.m_pDecl->AppendAttribute(AT_BoneWeight, 4, CT_Float);
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

	void BMWModel::render(glm::mat4 view, glm::mat4 proj, bool renderAlphas)
	{
		for (int i = 0; i < m_toRender.size(); i++) {
			if (renderAlphas == m_meshes[m_toRender[i].meshID].isTransparent) {
				renderMesh(transform * m_toRender[i].transform, view, proj, i);
			}
		}
		glDepthMask(true);
	}

	void BMWModel::renderMesh(glm::mat4 world, glm::mat4 view, glm::mat4 proj, unsigned int meshID) {
		static int frame = 0;

		m_meshes[meshID].m_pDecl->Bind();

		if (m_meshes[meshID].m_pTex != NULL)
			m_meshes[meshID].m_pTex->Bind();

		glm::mat4 boneMatrix[64];
		if (m_anims.size() > 0) {
			frame = (frame + 1) % m_anims[0]->duration;
			for (auto it : m_anims[0]->transforms) {
				boneMatrix[it.first] = it.second[frame];
			}
		}

		m_meshes[meshID].m_pProg->Bind();
		m_meshes[meshID].m_pProg->SetUniform("projection", proj);
		m_meshes[meshID].m_pProg->SetUniform("view", view);
		m_meshes[meshID].m_pProg->SetUniform("world", world);
		m_meshes[meshID].m_pProg->SetUniform("tex", 0);
		m_meshes[meshID].m_pProg->SetUniform("BoneMatrixArray", boneMatrix, 64);

		glDrawElements(GL_TRIANGLES, m_meshes[meshID].m_pIB->GetNumIndices(), GL_UNSIGNED_INT, 0);
	}

	glm::mat4 BMWModel::getTransform() {
		return transform;
	}

	void BMWModel::setTransform(glm::mat4 transform) {
		this->transform = transform;
	}
}