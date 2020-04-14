#include "math/W_RNG.h"

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
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(*gen());
	}

	float RNG::GetRandom(const float& min, const float& max)
	{
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(*gen());
	}

	double RNG::GetRandom(const double& min, const double& max)
	{
		std::uniform_real_distribution<double> distribution(min, max);
		return distribution(*gen());
	}

	std::mt19937* RNG::gen() {
		static std::random_device randomDevice;
		static std::mt19937 gen(randomDevice());
		return &gen;
	}
}