#include "QuadTree.h"

namespace wolf
{
	float clamp(float val, float min, float max)
	{
		if (val < min)
			return min;
		else if (val > max)
			return max;
		return val;
	}

	QuadTree::QuadTree(float xpos, float zpos, float width, float depth)
	{
		m_xpos = xpos;
		m_zpos = zpos;
		m_width = width;
		m_depth = depth;

		// points for debug rendering
		GLfloat points[] = { m_xpos, 0, m_zpos,
							 m_xpos, 0, m_zpos + m_depth,
							 m_xpos, 0, m_zpos,
							 m_xpos + m_width, 0, m_zpos,
							 m_xpos + m_width, 0, m_zpos + m_depth,
							 m_xpos + m_width, 0, m_zpos,
							 m_xpos + m_width, 0, m_zpos + m_depth,
							 m_xpos, 0, m_zpos + m_depth,
		};

		m_Program = ProgramManager::CreateProgram("../resources/shaders/line.vsh", "../resources/shaders/line.fsh");
		m_VB = BufferManager::CreateVertexBuffer(points, sizeof(GLfloat) * 3 * 8);
		m_Decl = new VertexDeclaration();
		m_Decl->Begin();
		m_Decl->AppendAttribute(AT_Position, 3, CT_Float);
		m_Decl->SetVertexBuffer(m_VB);
		m_Decl->End();
	}

	QuadTree::~QuadTree()
	{
		BufferManager::DestroyBuffer(m_VB);
		ProgramManager::DestroyProgram(m_Program);
		delete m_Decl;
	}

	bool QuadTree::AddNode(QuadTreeNode* child)
	{
		// check if node fully engulfs the quadtree
		if (isSphereEngulfing(child->node->getPos(), child->node->getCollisionRadius()))
		{
			m_bigNodes.push_back(child);
			return true;
		}

		// if the node isn't inside
		if (!IsSphereInside(child->node->getPos(), child->node->getCollisionRadius()))
			return false;

		// if there's already enough nodes, split into child trees
		if (m_nodes.size() >= 2 && m_children.size() == 0)
		{
			m_children.push_back(new QuadTree(m_xpos, m_zpos, m_width / 2, m_depth / 2));
			m_children.push_back(new QuadTree(m_xpos + m_width / 2, m_zpos, m_width / 2, m_depth / 2));
			m_children.push_back(new QuadTree(m_xpos + m_width / 2, m_zpos + m_depth / 2, m_width / 2, m_depth / 2));
			m_children.push_back(new QuadTree(m_xpos, m_zpos + m_depth / 2, m_width / 2, m_depth / 2));

			for (int i = 0; i < m_nodes.size(); i++)
			{
				for (int j = 0; j < m_children.size(); j++)
				{
					m_children[j]->AddNode(m_nodes[i]);
				}
			}
			m_nodes.clear();
		}

		// if there are existing child trees
		if (m_children.size() == 4)
		{
			bool toret = false;
			for (int i = 0; i < m_children.size(); i++)
				toret |= m_children[i]->AddNode(child);
			return toret;
		}

		// all else fails, add to list of nodes
		m_nodes.push_back(child);
	}

	bool QuadTree::IsSphereInside(glm::vec3 point, float radius)
	{
		float closestX = clamp(point.x, m_xpos, m_xpos + m_width);
		float closestZ = clamp(point.z, m_zpos, m_zpos + m_depth);

		float distanceX = point.x - closestX;
		float distanceZ = point.z - closestZ;

		return (distanceX * distanceX) + (distanceZ * distanceZ) < (radius * radius);
	}

	bool QuadTree::isSphereEngulfing(glm::vec3 point, float radius)
	{
		return ((m_xpos - point.x)*(m_xpos - point.x) + (m_zpos - point.z)*(m_zpos - point.z) < radius*radius) &&
			((m_xpos + m_width - point.x)*(m_xpos + m_width - point.x) + (m_zpos - point.z)*(m_zpos - point.z) < radius*radius) &&
			((m_xpos - point.x)*(m_xpos - point.x) + (m_zpos + m_depth - point.z)*(m_zpos + m_depth - point.z) < radius*radius) &&
			((m_xpos + m_width - point.x)*(m_xpos + m_width - point.x) + (m_zpos + m_depth - point.z)*(m_zpos + m_depth - point.z) < radius*radius);
	}

	bool QuadTree::IsFrustumInside(Plane* frustum)
	{
		for (int i = 0; i < 6; i++)
		{
			// skip the top/bottom planes, because we don't care about height
			// this makes the assumption that the camera won't rotate
			if (i == 2)
				i += 2;

			// check each point of the square
			bool cur = glm::dot(glm::vec4(frustum[i].a, frustum[i].b, frustum[i].c, frustum[i].d), glm::vec4(m_xpos, 0, m_zpos, 1)) > 0;
			cur |= glm::dot(glm::vec4(frustum[i].a, frustum[i].b, frustum[i].c, frustum[i].d), glm::vec4(m_xpos + m_width, 0, m_zpos, 1)) > 0;
			cur |= glm::dot(glm::vec4(frustum[i].a, frustum[i].b, frustum[i].c, frustum[i].d), glm::vec4(m_xpos + m_width, 0, m_zpos + m_depth, 1)) > 0;
			cur |= glm::dot(glm::vec4(frustum[i].a, frustum[i].b, frustum[i].c, frustum[i].d), glm::vec4(m_xpos, 0, m_zpos + m_depth, 1)) > 0;

			// if all are out, none work
			if (!cur)
				return false;
		}

		return true;
	}

	std::vector<Node*> QuadTree::GetVisible(long currentRender, Plane* cull)
	{
		std::vector<Node*> nodes;

		if (!IsFrustumInside(cull))
			return nodes;

		for (int i = 0; i < m_nodes.size(); i++)
		{
			if (m_nodes[i]->node->IsVisible(cull) && m_nodes[i]->lastRendered < currentRender)
			{
				m_nodes[i]->lastRendered = currentRender;
				nodes.push_back(m_nodes[i]->node);
			}
		}

		for (int i = 0; i < m_bigNodes.size(); i++)
		{
			if (m_bigNodes[i]->node->IsVisible(cull) && m_bigNodes[i]->lastRendered < currentRender)
			{
				m_bigNodes[i]->lastRendered = currentRender;
				nodes.push_back(m_bigNodes[i]->node);
			}
		}

		for (int i = 0; i < m_children.size(); i++)
		{
			std::vector<Node*> current = m_children[i]->GetVisible(currentRender, cull);
			for (int i = 0; i < current.size(); i++)
				nodes.push_back(current[i]);
		}

		return nodes;
	}

	void QuadTree::DebugRender(glm::mat4 projview)
	{
		// bind things
		m_Program->Bind();
		m_Program->SetUniform("projection", projview);
		m_Decl->Bind();

		// draw things
		glDrawArrays(GL_LINES, 0, 3 * 8);

		for (int i = 0; i < m_children.size(); i++)
			m_children[i]->DebugRender(projview);
	}
}