#include "Node.h"

namespace wolf
{
	Node::Node()
	{
		m_radius = 1.0f;
	}

	Node::~Node()
	{

	}

	void Node::Update(float delta)
	{
		for (int i = 0; i < m_children.size(); i++)
		{
			m_children[i]->Update(delta);
		}
	}

	void Node::AddChild(Node* child)
	{
		child->parent = this;
		m_children.push_back(child);
	}

	void Node::Render(glm::mat4 projview)
	{

	}

	bool Node::IsVisible(Plane* cull)
	{
		for (int i = 0; i < 6; i++)
		{
			if (glm::dot(glm::vec4(cull[i].a, cull[i].b, cull[i].c, cull[i].d), glm::vec4(m_position.x, m_position.y, m_position.z, 1)) < -m_radius)
				return false;
		}

		return true;
	}

	void Node::setPos(glm::vec3 pos)
	{
		m_position = pos;
	}

	glm::vec3 Node::getPos()
	{
		return m_position;
	}

	void Node::setScale(glm::vec3 scale)
	{
		m_scale = scale;

		m_radius = glm::max(glm::max(scale.x, scale.y), scale.z) * sqrt(2);
	}

	glm::vec3 Node::getScale()
	{
		return m_scale;
	}

	void Node::setRot(glm::quat rot)
	{
		m_rotation = rot;
	}

	glm::quat Node::getRot()
	{
		return m_rotation;
	}

	float Node::getCollisionRadius()
	{
		return m_radius;
	}
}

