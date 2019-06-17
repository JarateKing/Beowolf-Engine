#include "W_Math.h"
#include "W_RNG.h"

namespace wolf
{
	ull mulmodulo(ull a, ull b, ull m) {
		ull x = 0;
		ull y = a % m;
		while (b > 0) {
			if (b & 1 == 1) {
				x = (x + y) % m;
			}
			y = (y * 2) % m;
			b /= 2;
		}
		return x % m;
	}
	ull modulo(ull base, ull e, ull m) {
		ull x = 1;
		ull y = base;
		while (e > 0) {
			if (e & 1 == 1)
				x = (x * y) % m;
			y = (y * y) % m;
			e = e / 2;
		}
		return x % m;
	}

	// Miller-Rabin Primality Test
	bool Math::isPrime(ull num)
	{
		return isPrime(num, 10);
	}

	bool Math::isPrime(ull num, int iterations)
	{
		if (num < 2) return false;
		if (num == 2) return true;
		if (num & 1 == 0) return false;

		ull s = num - 1;
		while (s % 2 == 0) s /= 2;

		for (int i = 0; i < iterations; i++) {
			ull temp = s;
			ull a = RNG::GetRandom(1, num);
			ull mod = modulo(a, temp, num);
			while (temp != num - 1 && mod != 1 && mod != num - 1) {
				mod = mulmodulo(mod, mod, num);
				temp *= 2;
			}
			if (mod != num - 1 && temp & 1 == 0) {
				return false;
			}
		}
		return true;
	}


	float Math::lerp(float start, float end, float percent)
	{
		return start * (1 - percent) + end * (percent);
	}

	double Math::lerp(double start, double end, float percent)
	{
		return start * (1 - percent) + end * (percent);
	}

	glm::vec2 Math::lerp(glm::vec2 start, glm::vec2 end, float percent)
	{
		return start * (1 - percent) + end * (percent);
	}

	glm::vec3 Math::lerp(glm::vec3 start, glm::vec3 end, float percent)
	{
		return start * (1 - percent) + end * (percent);
	}
}