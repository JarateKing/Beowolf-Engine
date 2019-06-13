#include "W_Common.h"

namespace wolf
{
	class Math
	{
	public:
		static bool isPrime(unsigned long long num);
		static float lerp(float start, float end, float percent);
		static double Math::lerp(double start, double end, float percent);
	};
}