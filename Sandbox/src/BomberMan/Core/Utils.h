#pragma once

#include <random>
#include <glm/glm.hpp>

namespace Utils
{
	static inline bool Intersect(const glm::vec3& a, const glm::vec3& b)
	{
		return a.x > b.x - 0.5f && a.x < b.x + 0.5f &&
			a.y > b.y - 0.5f && a.y < b.y + 0.5f;
	}

	static inline float Lerp(float start, float end, float maxDistanceDelta)
	{
		if (glm::abs(end - start) <= maxDistanceDelta)
			return end;

		return start + glm::sign(end - start) * maxDistanceDelta;
	}

	static inline float RoundUp(float number)
	{
		float rounded = glm::round(number);

		if (rounded < number)
		{
			return rounded + 1;
		}

		return rounded;
	}

	static inline unsigned int Random(int min, int max)
	{
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> distribution(min, max);

		return distribution(rng);
	}
}