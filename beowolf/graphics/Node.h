#ifndef NODE_H
#define NODE_H

#include "W_Common.h"
#include <vector>

namespace wolf
{
	class Node
	{
	public:
		Node();
		~Node();
		virtual void Update(float delta);
		virtual void Render(glm::mat4 projview);
		virtual bool IsVisible(Plane* cull);
		void AddChild(Node* child);

		void setPos(glm::vec3 pos);
		glm::vec3 getPos();
		void setScale(glm::vec3 scale);
		glm::vec3 getScale();
		void setRot(glm::quat rot);
		glm::quat getRot();
		float getCollisionRadius();

		Node* parent;

	private:
		std::vector<Node*> m_children;
		glm::vec3 m_position;
		glm::vec3 m_scale;
		glm::quat m_rotation;
		float m_radius;
	};
}

#endif