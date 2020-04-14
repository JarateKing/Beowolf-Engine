#include "BMWModel.h"
#include <iostream>
#include <iomanip>
#include <stack>
#include "W_BufferManager.h"
#include "W_Input.h"
#include "W_Math.h"
#include "W_ResourceLoader.h"

namespace wolf
{
	BMWModel::BMWModel(const std::string& file, const std::string& vertexShader, const std::string& pixelShader, const std::string& shadowVertexShader, const std::string& shadowPixelShader)
	{
		BMWModeLData* data = BMWLoader::getInstance().loadFile(file);

		m_anims = &data->animlist;
		m_animFrames = &data->animations;
		m_defaultAnimation = &data->defaultAnim;
		m_vertices = &data->meshlist;
		m_textures = &data->texlist;
		m_rootNode = data->root;
		m_indices = &data->indexlist;
		m_transformModel = data->transform;

		m_hasAnimations = m_animFrames->size() > 0;
		if (m_hasAnimations) {
			m_currentAnimation = (*m_animFrames)[*m_defaultAnimation];
			m_animationFrame = m_currentAnimation->start;
			m_currentAnimNum = m_currentAnimation->anim;
		}

		// set up m_meshes
		for (int i = 0; i < m_vertices->size(); i++) {
			Mesh current;

			current.size = m_vertices[i].size();
			current.m_pVB = wolf::BufferManager::CreateVertexBuffer(&((*m_vertices)[i][0]), sizeof(Vertex) * (*m_vertices)[i].size());

			current.m_pIB = wolf::BufferManager::CreateIndexBuffer((*m_indices)[i].size());
			current.m_pIB->Write(&((*m_indices)[i][0]), (*m_indices)[i].size() * sizeof(unsigned int));

			current.m_pProg = wolf::ProgramManager::CreateProgram(vertexShader, pixelShader);

			auto animateShadowShaders = wolf::ResourceLoader::Instance().getShaders("shadow_map_animate");

			if(m_hasAnimations)
				current.m_pShadowProg = wolf::ProgramManager::CreateProgram(animateShadowShaders.first, animateShadowShaders.second);
			else
				current.m_pShadowProg = wolf::ProgramManager::CreateProgram(shadowVertexShader, shadowPixelShader);

			current.m_pTex = NULL;
			if (m_textures->size() > i) {
				bool transparency = false; // TODO: improve, naive assumption that jpg's always mean opaque and png's always mean transparent
				if ((*m_textures)[i].find(".jpg") != std::string::npos)
					(*m_textures)[i] = (*m_textures)[i].substr(0, (*m_textures)[i].find(".jpg")) + std::string(".dds");
				else if ((*m_textures)[i].find(".png") != std::string::npos) {
					(*m_textures)[i] = (*m_textures)[i].substr(0, (*m_textures)[i].find(".png")) + std::string(".dds");
					transparency = true;
				}

				current.m_pTex = wolf::TextureManager::CreateTexture(wolf::ResourceLoader::Instance().getModelTexture((*m_textures)[i]));
				current.m_pTex->SetWrapMode(wolf::Texture::WM_Repeat);
				current.isTransparent = transparency;

				// hotfix for multiple duplicate models
				if (transparency)
					(*m_textures)[i] = (*m_textures)[i].substr(0, (*m_textures)[i].find(".dds")) + std::string(".png");
			}

			current.m_pDecl = new wolf::VertexDeclaration();
			current.m_pDecl->Begin();
			Vertex::applyAttributes(current.m_pDecl);
			current.m_pDecl->SetVertexBuffer(current.m_pVB);
			current.m_pDecl->SetIndexBuffer(current.m_pIB);
			current.m_pDecl->End();

			m_meshes.push_back(current);
		}

		std::stack<BMWNode*> nodes;
		nodes.push(m_rootNode);
		while (!nodes.empty()) {
			{
				BMWNode* current = nodes.top();
				nodes.pop();

				for (int i = 0; i < current->children.size(); i++) {
					nodes.push(current->children[i]);
				}

				for (int i = 0; i < current->meshNum; i++) {
					NodeMesh renderable;
					renderable.transform = current->transform;
					renderable.meshID = current->meshIDs[i];
					m_toRender.push_back(renderable);
				}
			}
		}

		transform = m_transformModel;
	}

	BMWModel::~BMWModel() {
		for (int i = 0; i < m_meshes.size(); i++) {
			wolf::BufferManager::DestroyBuffer(m_meshes[i].m_pVB);
			wolf::BufferManager::DestroyBuffer(m_meshes[i].m_pIB);
			wolf::ProgramManager::DestroyProgram(m_meshes[i].m_pProg);
			wolf::ProgramManager::DestroyProgram(m_meshes[i].m_pShadowProg);
			wolf::TextureManager::DestroyTexture(m_meshes[i].m_pTex);
			delete m_meshes[i].m_pDecl;
		}
	}

	void BMWModel::update(float delta)
	{
		if (m_hasAnimations) {
			int prev = m_animationFrame;

			m_animationFrame += delta * (*m_anims)[m_currentAnimNum]->rate;
			if (m_animationFrame >= m_currentAnimation->end && m_currentAnimation->isHold) {
				m_animationFrame = m_currentAnimation->end;
				m_isAnimationDone = true;
			}

			if (prev != (int)(m_animationFrame)) {

				if (m_animationFrame > m_currentAnimation->end && !m_currentAnimation->isLoop)
					setAnim(*m_defaultAnimation);

				m_animationFrame = wolf::Math::wrap(m_animationFrame, m_currentAnimation->start, m_currentAnimation->end);

				for (auto it : (*m_anims)[m_currentAnimNum]->transforms[m_animationFrame]) {
					if (it.first < 128)
						m_boneMatrix[it.first] = it.second;
				}
			}
		}
	}

	void BMWModel::render(glm::mat4 view, glm::mat4 proj, glm::mat4 lightSpaceMatrix, RenderFilterType type, bool shadowPass, unsigned int depthMapTexture, bool instanced, float minheight, float maxheight)
	{
		if (type == RenderFilterOpaque)
			for (int i = 0; i < m_toRender.size(); i++)
				if (!m_meshes[m_toRender[i].meshID].isTransparent)
					renderMesh(transform * m_toRender[i].transform, view, proj, lightSpaceMatrix, i, shadowPass, depthMapTexture, instanced, minheight, maxheight);

		if (type == RenderFilterTransparent)
			for (int i = 0; i < m_toRender.size(); i++)
				if (m_meshes[m_toRender[i].meshID].isTransparent)
					renderMesh(transform * m_toRender[i].transform, view, proj, lightSpaceMatrix, i, shadowPass, depthMapTexture, instanced, minheight, maxheight);
	}

	void BMWModel::renderMesh(glm::mat4 world, glm::mat4 view, glm::mat4 proj, glm::mat4 lightSpaceMatrix, unsigned int meshID, bool shadowPass, unsigned int depthMapTexture, bool instanced, float minheight, float maxheight) {
		m_meshes[meshID].m_pDecl->Bind();

		glActiveTexture(GL_TEXTURE0);
		if (m_meshes[meshID].m_pTex != NULL)
			m_meshes[meshID].m_pTex->Bind();

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glActiveTexture(GL_TEXTURE0);

		if (shadowPass)
		{
			m_meshes[meshID].m_pShadowProg->Bind();
			m_meshes[meshID].m_pShadowProg->SetUniform("lightSpaceMatrix", lightSpaceMatrix);
			m_meshes[meshID].m_pShadowProg->SetUniform("model", transform);
		}
		else if (instanced)
		{
			m_meshes[meshID].m_pProg->Bind();
			m_meshes[meshID].m_pProg->SetUniform("projection", proj);
			m_meshes[meshID].m_pProg->SetUniform("view", view);

			m_meshes[meshID].m_pProg->SetUniform("world", m_instancedWorld, 45);

			m_meshes[meshID].m_pProg->SetUniform("tex", 0);
			m_meshes[meshID].m_pProg->SetUniform("shadowMap", 1);
			m_meshes[meshID].m_pProg->SetUniform("modelColor", m_modelColor);
			m_meshes[meshID].m_pProg->SetUniform("modelAdditive", m_modelAdditive);
			m_meshes[meshID].m_pProg->SetUniform("modelFilter", m_modelFilter);
			m_meshes[meshID].m_pProg->SetUniform("LightAmbient", m_lightAmbient);
			m_meshes[meshID].m_pProg->SetUniform("LightDiffuse", m_lightDiffuse);
			m_meshes[meshID].m_pProg->SetUniform("LightDir", m_lightDir);
			m_meshes[meshID].m_pProg->SetUniform("ViewDir", m_viewDir);
			m_meshes[meshID].m_pProg->SetUniform("lightSpaceMatrix", lightSpaceMatrix);
			glm::mat3 mWorldIT(transform);
			mWorldIT = glm::inverse(mWorldIT);
			mWorldIT = glm::transpose(mWorldIT);
			m_meshes[meshID].m_pProg->SetUniform("WorldIT", mWorldIT);
		}
		else
		{
			m_meshes[meshID].m_pProg->Bind();
			m_meshes[meshID].m_pProg->SetUniform("projection", proj);
			m_meshes[meshID].m_pProg->SetUniform("view", view);
			m_meshes[meshID].m_pProg->SetUniform("world", transform);
			m_meshes[meshID].m_pProg->SetUniform("tex", 0);
			m_meshes[meshID].m_pProg->SetUniform("shadowMap", 1);
			m_meshes[meshID].m_pProg->SetUniform("modelColor", m_modelColor);
			m_meshes[meshID].m_pProg->SetUniform("modelAdditive", m_modelAdditive);
			m_meshes[meshID].m_pProg->SetUniform("modelFilter", m_modelFilter);
			m_meshes[meshID].m_pProg->SetUniform("LightAmbient", m_lightAmbient);
			m_meshes[meshID].m_pProg->SetUniform("LightDiffuse", m_lightDiffuse);
			m_meshes[meshID].m_pProg->SetUniform("LightDir", m_lightDir);
			m_meshes[meshID].m_pProg->SetUniform("ViewDir", m_viewDir);
			m_meshes[meshID].m_pProg->SetUniform("lightSpaceMatrix", lightSpaceMatrix);
			glm::mat3 mWorldIT(transform);
			mWorldIT = glm::inverse(mWorldIT);
			mWorldIT = glm::transpose(mWorldIT);
			m_meshes[meshID].m_pProg->SetUniform("WorldIT", mWorldIT);
		}

		m_meshes[meshID].m_pProg->SetUniform("minheight", minheight);
		m_meshes[meshID].m_pProg->SetUniform("maxheight", maxheight);

		if (m_hasAnimations)
			m_meshes[meshID].m_pProg->SetUniform("BoneMatrixArray", m_boneMatrix, 128);

		if (instanced)
			glDrawElementsInstanced(GL_TRIANGLES, m_meshes[meshID].m_pIB->GetNumIndices(), GL_UNSIGNED_INT, 0, 45);
		else
			glDrawElements(GL_TRIANGLES, m_meshes[meshID].m_pIB->GetNumIndices(), GL_UNSIGNED_INT, 0);
	}

	glm::mat4 BMWModel::getTransform() {
		return transform;
	}

	void BMWModel::setTransform(const glm::mat4& transform) {
		this->transform = transform * m_transformModel;
	}

	void BMWModel::setAnim(const std::string& name) {
		if (m_hasAnimations && m_animFrames->count(name)) {
			m_currentAnimation = (*m_animFrames)[name];
			m_animationFrame = m_currentAnimation->start;
			m_currentAnimNum = m_currentAnimation->anim;
			m_isAnimationDone = false;
		}
		else if (m_hasAnimations) {
			std::cout << "Attempted setting \"" << name << "\" animation, but does not exist!\n";
		}
		else {
			std::cout << "Attempted setting \"" << name << "\" animation, but animations not supported on this model!\n";
		}
	}

	void BMWModel::setModelColor(const glm::vec3& color) {
		m_modelColor = color;
	}

	void BMWModel::setModelAdditive(const glm::vec3& color) {
		m_modelAdditive = color;
	}

	void BMWModel::setModelFilter(const glm::vec3& color) {
		m_modelFilter = color;
	}

	void BMWModel::setLightAmbient(const glm::vec4& light) {
		m_lightAmbient = light;
	}

	void BMWModel::setLightDiffuse(const glm::vec4& light) {
		m_lightDiffuse = light;
	}

	void BMWModel::setLightSpecular(const glm::vec4& light) {
		m_lightSpecular = light;
	}

	void BMWModel::setLightDir(const glm::vec3& direction) {
		m_lightDir = direction;
	}

	void BMWModel::setViewDir(const glm::vec3& direction) {
		m_viewDir = direction;
	}

	void BMWModel::setInstancedVariable(const std::vector<glm::mat4>& instancedV)
	{
		for (int i = 0; i < instancedV.size(); i++)
		{
			m_instancedWorld[i] = instancedV.at(i);
		}
	}

	bool BMWModel::getIsAnimationRunning() {
		return !m_isAnimationDone;
	}

	BMWAnimSegment* BMWModel::getAnim() {
		return m_currentAnimation;
	}

	std::string BMWModel::getAnimName() {
		for (auto it : (*m_animFrames))
			if (it.second == m_currentAnimation)
				return it.first;
		return "";
	}

	bool BMWModel::isAnimDefault() {
		return m_currentAnimation == (*m_animFrames)[*m_defaultAnimation];
	}

	bool BMWModel::canAnimate()
	{
		return m_hasAnimations;
	}
}