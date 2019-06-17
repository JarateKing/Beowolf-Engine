#ifndef W_MATH_H
#define W_MATH_H

#include "W_Common.h"

namespace wolf
{
	class Math
	{
	public:
		static bool isPrime(ull num);
		static bool isPrime(ull num, int iterations);
		static float lerp(float start, float end, float percent);
		static double lerp(double start, double end, float percent);
	};
}

#endif