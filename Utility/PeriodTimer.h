#pragma once

#include <chrono>

class PeriodTimer
{
public:
	PeriodTimer()
		:tpStart(std::chrono::steady_clock::now())
	{

	}
	~PeriodTimer() = default;
public:
	void restart()
	{
		tpStart = std::chrono::steady_clock::now();
	}

	std::chrono::milliseconds elapsed()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - tpStart);
	}

	bool isLargerThan(const std::chrono::milliseconds& msec) const
	{
		return std::chrono::steady_clock::now() - tpStart >= msec;
	}
private:
	std::chrono::steady_clock::time_point tpStart;
};

