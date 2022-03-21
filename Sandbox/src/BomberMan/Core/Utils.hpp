#pragma once

#include <random>
#include <functional>

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

	template<typename T>
	static T Random(T min, T max)
	{
		static_assert(false);
	}

	template<>
	static int Random(int min, int max)
	{
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> distribution(min, max);

		return distribution(rng);
	}

	template<>
	static float Random(float min, float max)
	{
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_real_distribution<float> distribution(min, max);

		return distribution(rng);
	}

	template<typename... Args>
	class Timer
	{
	public:
		Timer() {}
		~Timer() {}

		void Create(float desired, const std::function<void(float)>& until, const std::function<void(Args...)>& reached)
		{
			m_DesiredTime = desired;
			m_DoUntilFunc = until;
			m_DoWhenReached = reached;
		}

		void Tick(float ts, Args ...args)
		{
			if (m_EstimatedTime < m_DesiredTime)
			{
				m_DoUntilFunc(ts);

				m_EstimatedTime += ts;

				if (m_EstimatedTime > m_DesiredTime)
				{
					m_DoWhenReached(args...);
				}
			}
		}

		void Reset()
		{
			m_EstimatedTime = 0.0f;
		}

	private:
		Timer(const Timer&) = delete;

		std::function<void(float)> m_DoUntilFunc;
		std::function<void(Args...)> m_DoWhenReached;

		float m_DesiredTime{ 0.0f };
		float m_EstimatedTime{ 0.0f };
	};
}