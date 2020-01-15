#ifndef EFFECT_H
#define EFFECT_H

#include "W_Common.h"
#include "Emitter.h"
#include <vector>

class Effect
{
public:
	Effect();
	Effect(std::string jsonPath);
	~Effect();
	void Update(float delta, glm::mat3 view);
	void Render(glm::mat4 projview, wolf::RenderFilterType type);

	glm::vec3 GetPos();
	void SetPos(glm::vec3 pos);

private:
	std::vector<Emitter*> m_emitters;
	glm::vec3 m_pos;
};

#endif