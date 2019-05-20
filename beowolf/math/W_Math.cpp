#include "W_Math.h"
#include "W_RNG.h"

namespace wolf
{
	unsigned long long mulmodulo(unsigned long long a, unsigned long long b, unsigned long long m) {
		unsigned long long x = 0;
		unsigned long long y = a % m;
		while (b > 0) {
			if (b % 2 == 1) {
				x = (x + y) % m;
			}
			y = (y * 2) % m;
			b /= 2;
		}
		return x % m;
	}
	unsigned long long modulo(unsigned long long base, unsigned long long e, unsigned long long m) {
		unsigned long long x = 1;
		unsigned long long y = base;
		while (e > 0) {
			if (e % 2 == 1)
				x = (x * y) % m;
			y = (y * y) % m;
			e = e / 2;
		}
		return x % m;
	}

	// Miller-Rabin Primality Test
	bool Math::isPrime(unsigned long long num)
	{
		if (num < 2) return false;
		if (num == 2) return true;
		if (num % 2 == 0) return false;

		unsigned long long s = num - 1;
		while (s % 2 == 0) s /= 2;

		for (int i = 0; i < 10; i++) {
			unsigned long long temp = s;
			unsigned long long a = RNG::GetRandom(1, num);
			unsigned long long mod = modulo(a, temp, num);
			while (temp != num - 1 && mod != 1 && mod != num - 1) {
				mod = mulmodulo(mod, mod, num);
				temp *= 2;
			}
			if (mod != num - 1 && temp % 2 == 0) {
				return false;
			}
		}
		return true;
	}


	float Math::lerp(float start, float end, float percent)
	{
		return start * (1 - percent) + end * (percent);
	}
}