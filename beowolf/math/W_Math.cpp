#include "W_Math.h"
#include "W_RNG.h"
#include <math.h> 

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
		return fmaf(percent, end, fmaf(-percent, start, start));
	}

	double Math::lerp(double start, double end, float percent)
	{
		return fma(percent, end, fma(-percent, start, start));
	}

	glm::vec2 Math::lerp(glm::vec2 start, glm::vec2 end, float percent)
	{
		return start * (1 - percent) + end * (percent);
	}

	glm::vec3 Math::lerp(glm::vec3 start, glm::vec3 end, float percent)
	{
		return start * (1 - percent) + end * (percent);
	}

	glm::vec2 Math::nlerp(glm::vec2 start, glm::vec2 end, float percent)
	{
		return glm::normalize(lerp(start, end, percent));
	}

	glm::vec3 Math::nlerp(glm::vec3 start, glm::vec3 end, float percent)
	{
		return glm::normalize(lerp(start, end, percent));
	}

	glm::vec2 Math::slerp(glm::vec2 start, glm::vec2 end, float percent)
	{
		return glm::vec2(slerp(glm::vec3(start.x, start.y, 0), glm::vec3(end.x, end.y, 0), percent));
	}

	glm::vec3 Math::slerp(glm::vec3 start, glm::vec3 end, float percent)
	{
		return glm::eulerAngles(slerp(toQuaternion(start), toQuaternion(end), percent));
	}

	glm::quat Math::slerp(glm::quat start, glm::quat end, float percent)
	{
		return glm::mix(start, end, percent);
	}

	glm::quat Math::toQuaternion(glm::vec3 angles)
	{
		glm::quat pitch = glm::angleAxis(angles.x, glm::vec3(1, 0, 0));
		glm::quat roll = glm::angleAxis(angles.y, glm::vec3(0, 1, 0));
		glm::quat yaw = glm::angleAxis(angles.z, glm::vec3(0, 0, 1));
		return pitch * roll * yaw;
	}

	int Math::wrap(int val, int min, int max)
	{
		int range = max - min + 1;

		// wrap it in terms of min
		if (val < min)
			val += range * ((min - val) / range + 1);

		// wrap it in terms of max
		return min + (val - min) % range;
	}
}