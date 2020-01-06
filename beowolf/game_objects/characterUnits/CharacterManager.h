#ifndef CHARACTERMANAGER_H
#define CHARACTERMANAGER_H

#include <list>
#include "W_Common.h"
#include "BMWModel.h"

class CharacterManager
{
	public:
		CharacterManager();
		~CharacterManager();
		void Update(float deltaT);
		void Render(glm::mat4 p_view, glm::mat4 p_proj, bool p_renderAlphas);

	private:
		std::list<wolf::BMWModel> characters;

};

#endif
