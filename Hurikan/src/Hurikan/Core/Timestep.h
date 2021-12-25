#pragma once
namespace Hurikan 
{
	class Timestep 
	{
	public:
		Timestep(float time = 0.0f) : m_Time(time) {}
		operator float() const { return m_Time; }
		inline float GetSeconds() const { return m_Time; }
		inline float GetMilliseconds() const { return m_Time * 1000; }
		//inline float GetStartTime() const { return m_StartTime; }
	private:
		float m_Time;
		//float m_StartTime;
	};
	
}