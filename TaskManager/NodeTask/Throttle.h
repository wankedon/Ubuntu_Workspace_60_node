#pragma once

#include <chrono>
#include "Logger.h"

namespace DSP
{
	template <class T>
	class Throttle
	{
	public:
		Throttle(double sr)
			:sampleRate(sr),
			tpStart(std::chrono::steady_clock::now()),
			samplePeriod(std::chrono::duration<double>(1 / sr))
		{
		}
		~Throttle() = default;
	public:
		void start()
		{
			tpStart = std::chrono::steady_clock::now();
		}

		void work(size_t numOutputItems, const void* input, void* output)
		{
			if (input != nullptr && output != nullptr)
			{
				std::memcpy(output, input, numOutputItems * sizeof(T));
			}
			auto now = std::chrono::steady_clock::now();
			auto span = samplePeriod * numOutputItems;
			auto expectedTime = tpStart + span;
			//CLOG("throttle sleep {} ms", chrono::duration_cast<chrono::milliseconds>(span).count());
			if (expectedTime > now) {
				auto limitDuration =
					std::chrono::duration<double>(std::numeric_limits<long>::max());
				if (expectedTime - now > limitDuration) {
					CLOG("WARNING: Throttle sleep time overflow! You are probably using a very low sample rate.");
				}
				std::this_thread::sleep_until(expectedTime);
			}
		}

		void setSampleRate(double rate)
		{
			sampleRate = rate;
			samplePeriod = std::chrono::duration<double>(1 / rate);
			tpStart = std::chrono::steady_clock::now();
		}
	private:
		std::chrono::time_point<std::chrono::steady_clock> tpStart;
		double sampleRate;
		std::chrono::duration<double> samplePeriod;
	};
}


