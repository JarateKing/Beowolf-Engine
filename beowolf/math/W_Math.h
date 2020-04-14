#ifndef W_MATH_H
#define W_MATH_H

#include "W_Common.h"
#include "glm/gtx/quaternion.hpp"

namespace wolf
{
	class Math
	{
	public:
		static bool isPrime(const ull& num);
		static bool isPrime(const ull& num, const int& iterations);

		static float lerp(const float& start, const float& end, const float& percent);
		static double lerp(const double& start, const double& end, const float& percent);
		static glm::vec2 lerp(const glm::vec2& start, const glm::vec2& end, const float& percent);
		static glm::vec3 lerp(const glm::vec3& start, const glm::vec3& end, const float& percent);

		static glm::vec2 nlerp(const glm::vec2& start, const glm::vec2& end, const float& percent);
		static glm::vec3 nlerp(const glm::vec3& start, const glm::vec3& end, const float& percent);

		static glm::vec2 slerp(const glm::vec2& start, const glm::vec2& end, const float& percent);
		static glm::vec3 slerp(const glm::vec3& start, const glm::vec3& end, const float& percent);
		static glm::quat slerp(const glm::quat& start, const glm::quat& end, const float& percent);

		static glm::quat toQuaternion(const glm::vec3& angles);

		static int wrap(int val, const int& min, const int& max);
		static float wrap(float val, const float& min, const float& max);
		static int getSign(const int& val);

		static float easeIn(const float& percent);
		static float easeOut(const float& percent);
		static float ease(const float& percent);

	private:
		static ull Math::mulmodulo(ull a, ull b, const ull& m);
		static ull Math::modulo(ull a, ull b, const ull& m);
	};
}

#endif