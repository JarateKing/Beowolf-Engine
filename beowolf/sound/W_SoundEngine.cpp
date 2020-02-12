#include "W_SoundEngine.h"
#include <iostream>

namespace wolf
{
	SoundEngine::SoundEngine()
	{
	}

	SoundEngine::~SoundEngine()
	{

	}

	//Initializes The System
	void SoundEngine::InitSystem()
	{
		Common_Init(&extradriverdata);

		result = FMOD::System_Create(&system);
		ERRCHECK(result);

		result = system->getVersion(&version);
		ERRCHECK(result);

		if (version < FMOD_VERSION)
		{
			Common_Fatal("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
		}

		//Sets number of channels and system settings
		result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
		ERRCHECK(result);

		result = system->set3DSettings(1.0, DISTANCEFACTOR, 1.0f);
		ERRCHECK(result);
	}

	//Adds sound to stored sounds
	void SoundEngine::AddSound(const char* soundPath, const char* soundName, bool stereo)
	{
		//Creates stereo sound
		if (stereo)
		{
			result = system->createSound(soundPath, FMOD_2D, 0, &sound);
			ERRCHECK(result);
		}
		//creates 3D positionable sound
		else
		{
			result = system->createSound(soundPath, FMOD_3D, 0, &sound);
			ERRCHECK(result);
		}

		result = sound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 1000.0f * DISTANCEFACTOR);
		ERRCHECK(result);

		sounds.push_back(sound);
		soundNames.push_back(soundName);
	}

	//Plays basic stereo sound once
	bool SoundEngine::PlayBasicSound(const char* soundName)
	{
		std::vector<const char*>::iterator it = std::find(soundNames.begin(), soundNames.end(), soundName);

		if (it == soundNames.end())
		{
			std::cout << "Sound Not Found" << std::endl;
			return false;
		}

		int index = std::distance(soundNames.begin(), it);

		FMOD::Sound* soundPlayed = sounds.at(index);

		result = soundPlayed->setMode(FMOD_LOOP_OFF);
		ERRCHECK(result);

		result = system->playSound(soundPlayed, 0, false, &channel);
		ERRCHECK(result);

		return true;
	}

	//Plays a positionable sound
	bool SoundEngine::Play3DSound(const char* soundName, glm::vec3 position, glm::vec3 velocity, bool looped)
	{
		std::vector<const char*>::iterator it = std::find(soundNames.begin(), soundNames.end(), soundName);

		if (it == soundNames.end())
		{
			std::cout << "Sound Not Found" << std::endl;
			return false;
		}

		int index = std::distance(soundNames.begin(), it);

		FMOD::Sound* soundPlayed = sounds.at(index);

		FMOD_VECTOR pos = { position.x * DISTANCEFACTOR, position.y * DISTANCEFACTOR, position.z * DISTANCEFACTOR };
		FMOD_VECTOR vel = { velocity.x, velocity.y, velocity.z };

		result = channel->set3DAttributes(&pos, &vel);

		//Loops sound
		if (looped)
		{
			result = soundPlayed->setMode(FMOD_LOOP_NORMAL);
			ERRCHECK(result);
		}
		//Plays sound once
		else
		{
			result = soundPlayed->setMode(FMOD_LOOP_OFF);
			ERRCHECK(result);
		}

		result = system->playSound(soundPlayed, 0, false, &channel);
		ERRCHECK(result);

		return true;
	}

	//Loops a stereo sound
	bool SoundEngine::PlayLoopSound(const char* soundName)
	{
		std::vector<const char*>::iterator it = std::find(soundNames.begin(), soundNames.end(), soundName);

		if (it == soundNames.end())
		{
			std::cout << "Sound Not Found" << std::endl;
			return false;
		}

		int index = std::distance(soundNames.begin(), it);

		FMOD::Sound* soundPlayed = sounds.at(index);

		result = soundPlayed->setMode(FMOD_LOOP_NORMAL);
		ERRCHECK(result);

		result = system->playSound(soundPlayed, 0, false, &channel);
		ERRCHECK(result);
		return true;
	}

	//Changes the position of a sound
	void SoundEngine::Change3DSoundPos(const char* soundName, glm::vec3 newPos, glm::vec3 newVelo)
	{
		std::vector<const char*>::iterator it = std::find(soundNames.begin(), soundNames.end(), soundName);

		if (it == soundNames.end())
		{
			std::cout << "Sound Not Found" << std::endl;
		}

		int index = std::distance(soundNames.begin(), it);

		FMOD::Sound* soundChanging = sounds.at(index);

		FMOD_VECTOR pos = { newPos.x * DISTANCEFACTOR, newPos.y * DISTANCEFACTOR, newPos.z * DISTANCEFACTOR };
		FMOD_VECTOR vel = { newVelo.x, newVelo.y, newVelo.z };

		result = channel->set3DAttributes(&pos, &vel);
		ERRCHECK(result);
	}

	//Sets the attributes for the listener
	void SoundEngine::SetListenerAttr(glm::vec3 lisPos, glm::vec3 lisVel, glm::vec3 lisForward, glm::vec3 lisUp)
	{
		FMOD_VECTOR pos = { lisPos.x, lisPos.y, lisPos.z };
		FMOD_VECTOR vel = { lisVel.x, lisVel.y, lisVel.z };
		FMOD_VECTOR forward = { lisForward.x, lisForward.y, lisForward.z };
		FMOD_VECTOR up = { lisUp.x, lisUp.y, lisUp.z };

		result = system->set3DListenerAttributes(0, &pos, &vel, &forward, &up);
		ERRCHECK(result);
	}

	//Calls update on the system
	void SoundEngine::UpdateSystem()
	{
		system->update();
	}

	//Finds the position of the listener
	FMOD_VECTOR SoundEngine::GetListenerPos()
	{
		FMOD_VECTOR pos, vel, forward, up;
		result = system->get3DListenerAttributes(0, &pos, &vel, &forward, &up);
		ERRCHECK(result);
		return pos;
	}

	//Gets sound position
	FMOD_VECTOR SoundEngine::GetSoundPos(const char* soundName)
	{
		FMOD_VECTOR pos, vel;
		std::vector<const char*>::iterator it = std::find(soundNames.begin(), soundNames.end(), soundName);

		if (it == soundNames.end())
		{
			std::cout << "Sound Not Found" << std::endl;
		}

		int index = std::distance(soundNames.begin(), it);

		FMOD::Sound* soundChanging = sounds.at(index);

		result = channel->get3DAttributes(&pos, &vel);
		ERRCHECK(result);

		return pos;
	}

	//Gets sound Velocity
	FMOD_VECTOR SoundEngine::GetSoundVel(const char* soundName)
	{
		FMOD_VECTOR pos, vel;
		std::vector<const char*>::iterator it = std::find(soundNames.begin(), soundNames.end(), soundName);

		if (it == soundNames.end())
		{
			std::cout << "Sound Not Found" << std::endl;
		}

		int index = std::distance(soundNames.begin(), it);

		FMOD::Sound* soundChanging = sounds.at(index);

		result = channel->get3DAttributes(&pos, &vel);
		ERRCHECK(result);

		return vel;
	}

	void SoundEngine::PauseSound(const char* soundName)
	{
		std::vector<const char*>::iterator it = std::find(soundNames.begin(), soundNames.end(), soundName);
		if (it == soundNames.end())
		{
			std::cout << "Sound Not Found" << std::endl;
		}

		int index = std::distance(soundNames.begin(), it);
		FMOD::Sound* soundChanging = sounds.at(index);

		result = system->playSound(soundChanging, 0, true, &channel);
		ERRCHECK(result);
	}
}
