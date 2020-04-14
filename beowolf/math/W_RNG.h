#ifndef W_RNG_H
#define W_RNG_H

namespace wolf
{
	class RNG
	{
	public:
		static bool Bool();
		static bool Bool(const float& trueChance);
		static int GetRandom(const int& min, const int& max);
		static float GetRandom(const float& min, const float& max);
		static double GetRandom(const double& min, const double& max);
	};
}

#endif