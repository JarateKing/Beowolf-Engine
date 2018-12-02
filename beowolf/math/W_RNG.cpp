#include "math/W_RNG.h"
#include <random>
#include <ctime>

namespace wolf
{
	int RNG::GetRandom(int min, int max)
	{
		std::random_device randomDevice;
		std::mt19937 gen(randomDevice());
		std::uniform_int_distribution<> distribution(min, max);
		return distribution(gen);
	}

	float RNG::GetRandom(float min, float max)
	{
		std::random_device randomDevice;
		std::mt19937 gen(randomDevice());
		std::uniform_real_distribution<> distribution(min, max);
		return distribution(gen);
	}
}