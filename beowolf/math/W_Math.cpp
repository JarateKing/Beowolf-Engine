#include "W_Math.h"
#include "W_RNG.h"
#include <math.h> 

namespace wolf
{
	// methods for primality test
	ull Math::mulmodulo(ull a, ull b, const ull& m) {
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
	ull Math::modulo(ull base, ull e, const ull& m) {
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
	bool Math::isPrime(const ull& num)
	{
		return isPrime(num, 10);
	}
	bool Math::isPrime(const ull& num, const int& iterations)
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


	// standard lerp
	float Math::lerp(const float& start, const float& end, const float& percent)
	{
		return fmaf(percent, end, fmaf(-percent, start, start));
	}
	double Math::lerp(const double& start, const double& end, const float& percent)
	{
		return fma(percent, end, fma(-percent, start, start));
	}
	glm::vec2 Math::lerp(const glm::vec2& start, const glm::vec2& end, const float& percent)
	{
		return start * (1 - percent) + end * (percent);
	}
	glm::vec3 Math::lerp(const glm::vec3& start, const glm::vec3& end, const float& percent)
	{
		return start * (1 - percent) + end * (percent);
	}

	// normalized vector lerp
	glm::vec2 Math::nlerp(const glm::vec2& start, const glm::vec2& end, const float& percent)
	{
		return glm::normalize(lerp(start, end, percent));
	}
	glm::vec3 Math::nlerp(const glm::vec3& start, const glm::vec3& end, const float& percent)
	{
		return glm::normalize(lerp(start, end, percent));
	}

	// spherical lerp
	glm::vec2 Math::slerp(const glm::vec2& start, const glm::vec2& end, const float& percent)
	{
		return glm::vec2(slerp(glm::vec3(start.x, start.y, 0), glm::vec3(end.x, end.y, 0), percent));
	}
	glm::vec3 Math::slerp(const glm::vec3& start, const glm::vec3& end, const float& percent)
	{
		return glm::eulerAngles(slerp(toQuaternion(start), toQuaternion(end), percent));
	}
	glm::quat Math::slerp(const glm::quat& start, const glm::quat& end, const float& percent)
	{
		return glm::mix(start, end, percent);
	}

	// quaternion conversion
	glm::quat Math::toQuaternion(const glm::vec3& angles)
	{
		glm::quat pitch = glm::angleAxis(angles.x, glm::vec3(1, 0, 0));
		glm::quat roll = glm::angleAxis(angles.y, glm::vec3(0, 1, 0));
		glm::quat yaw = glm::angleAxis(angles.z, glm::vec3(0, 0, 1));
		return pitch * roll * yaw;
	}

	// wrapping code
	int Math::wrap(int val, const int& min, const int& max)
	{
		int range = max - min + 1;

		// wrap it in terms of min
		if (val < min)
			val += range * ((min - val) / range + 1);

		// wrap it in terms of max
		return min + (val - min) % range;
	}
	float Math::wrap(float val, const float& min, const float& max)
	{
		// wrap it in terms of min
		while (val < min)
			val += min;

		// wrap it in terms of max
		while (val > max)
			val = val - max + min;

		return val;
	}

	// sign coefficient (1, 0, -1)
	int Math::getSign(const int& val)
	{
		if (val == 0) return 0;
		return (val > 0) * 2 - 1;
	}

	// lerp time modifiers
	float Math::easeIn(const float& percent)
	{
		return percent * percent * percent;
	}
	float Math::easeOut(const float& percent)
	{
		return pow(percent, 1.0 / 3.0);
	}
	float Math::ease(const float& percent)
	{
		return (1 - cos(PI * percent)) * 0.5;
	}
}