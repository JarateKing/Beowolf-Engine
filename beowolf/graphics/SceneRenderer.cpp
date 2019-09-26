#ifndef __APPLE__
#include <GL/glew.h>
#endif
#include <GL/glfw.h>SceneRenderer::
#include "W_Common.h"
#include "W_VertexBuffer.h"
#include "W_ProjectionMatrix.h"

#include "SceneRenderer.h"

namespace wolf
{
	void SceneRenderer::AddNode(Node* node)
	{
		m_nodes.push_back(node);
	}

	void SceneRenderer::Update(float delta, glm::mat4 cull)
	{
		static long currentRender = 1;

		// update all nodes
		for (int i = 0; i < m_nodes.size(); i++)
		{
			m_nodes[i]->Update(delta);
		}

		// set up nodes to render
		m_toRender.clear();
		wolf::Plane* planes = wolf::ProjMatrix::GetFrustum(cull);
		m_toRender = m_quadtree->GetVisible(currentRender++, planes);
		delete[] planes;
	}

	void SceneRenderer::Render(glm::mat4 projview)
	{
		for (int i = 0; i < m_toRender.size(); i++)
		{
			m_toRender[i]->Render(projview);
		}
	}

	void SceneRenderer::GenerateQuadtree(float xpos, float zpos, float width, float depth)
	{
		m_quadtree = new QuadTree(xpos, zpos, width, depth);
		for (int i = 0; i < m_nodes.size(); i++)
		{
			m_quadtree->AddNode(new QuadTreeNode(m_nodes[i]));
		}
	}

	QuadTree* SceneRenderer::GetQuadtree()
	{
		return m_quadtree;
	}
}