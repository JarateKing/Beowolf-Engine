#ifndef W_MATH_H
#define W_MATH_H

#include "W_Common.h"
#include "glm/gtx/quaternion.hpp"

namespace wolf
{
	class Math
	{
	public:
		static bool isPrime(ull num);
		static bool isPrime(ull num, int iterations);

		static float lerp(float start, float end, float percent);
		static double lerp(double start, double end, float percent);
		static glm::vec2 lerp(glm::vec2 start, glm::vec2 end, float percent);
		static glm::vec3 lerp(glm::vec3 start, glm::vec3 end, float percent);

		static glm::vec2 nlerp(glm::vec2 start, glm::vec2 end, float percent);
		static glm::vec3 nlerp(glm::vec3 start, glm::vec3 end, float percent);

		static glm::vec2 slerp(glm::vec2 start, glm::vec2 end, float percent);
		static glm::vec3 slerp(glm::vec3 start, glm::vec3 end, float percent);
		static glm::quat slerp(glm::quat start, glm::quat end, float percent);

		static glm::quat toQuaternion(glm::vec3 angles);

		static int wrap(int val, int min, int max);
		static float wrap(float val, float min, float max);
		static int getSign(int val);

		static float easeIn(float percent);
		static float easeOut(float percent);
		static float ease(float percent);
	};
}

#endif