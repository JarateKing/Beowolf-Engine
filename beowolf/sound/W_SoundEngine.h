//-----------------------------------------------------------------------------
// File:			W_SoundEngine.h
// Original Author:	Dylan McBride
//
// Common functions used for Sound Effects
//-----------------------------------------------------------------------------
#ifndef W_SOUND_H
#define W_SOUND_H

#include "fmod/core/inc/fmod.hpp"
#include "fmod/core/examples/common.h"
#include "W_Common.h"
#include <vector>

namespace wolf
{
	class SoundEngine
	{
	public:
		SoundEngine();
		~SoundEngine();

		void InitSystem();
		void UpdateSystem();
		void AddSound(const char* soundPath, const char* soundName, bool stereo);
		bool PlayBasicSound(const char* soundName);
		bool Play3DSound(const char* soundName, glm::vec3 position, glm::vec3 velocity, bool loopable);
		bool PlayLoopSound(const char* soundName);
		void Change3DSoundPos(const char* soundName, glm::vec3 newPos, glm::vec3 newVelo);
		void SetListenerAttr(glm::vec3 lisPos, glm::vec3 lisVel, glm::vec3 lisForward, glm::vec3 lisUp);
		void PauseSound(const char* soundName);
		FMOD_VECTOR GetListenerPos();
		FMOD_VECTOR GetSoundPos(const char* soundName);
		FMOD_VECTOR GetSoundVel(const char* soundName);

	private:
		FMOD::System				*system;
		std::vector<FMOD::Sound*>	sounds;
		std::vector<const char*>	soundNames;
		FMOD::Sound					*sound;
		FMOD::Channel				*channel = 0;
		FMOD_RESULT					result;
		unsigned int				version;
		void						*extradriverdata = 0;
		const float					DISTANCEFACTOR = 1.0f;
		glm::vec3 listVel{ 10.0f, 0.0f, 0.0f };
		glm::vec3 listFor{ 0.0f, 0.0f, 1.0f };
		glm::vec3 listUp{ 0.0f, 1.0f, 0.0f };
		glm::vec3 listPos{ -50.0f, 0.0f, 0.0f };
		glm::vec3 soundPos{ 0.0f, 0.0f, 0.0f };
		glm::vec3 soundVel{ 100.0f, 0.0f, 0.0f };

	};
}

#endif
