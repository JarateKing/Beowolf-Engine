#include "math/W_RNG.h"
#include <random>
#include <ctime>

namespace wolf
{
	bool RNG::Bool()
	{
		return GetRandom(0, 1) == 1;
	}

	bool RNG::Bool(const float& trueChance)
	{
		return GetRandom(0.0f, 1.0f) < trueChance;
	}

	int RNG::GetRandom(const int& min, const int& max)
	{
		std::random_device randomDevice;
		std::mt19937 gen(randomDevice());
		std::uniform_int_distribution<> distribution(min, max);
		return distribution(gen);
	}

	float RNG::GetRandom(const float& min, const float& max)
	{
		std::random_device randomDevice;
		std::mt19937 gen(randomDevice());
		std::uniform_real_distribution<> distribution(min, max);
		return distribution(gen);
	}

	double RNG::GetRandom(const double& min, const double& max)
	{
		std::random_device randomDevice;
		std::mt19937 gen(randomDevice());
		std::uniform_real_distribution<> distribution(min, max);
		return distribution(gen);
	}
}