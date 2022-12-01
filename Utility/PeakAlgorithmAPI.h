#pragma once

#ifdef PSA_EXPORTS
#define PEAK_SEARTCH_API __declspec(dllexport)
#else
#define PEAK_SEARTCH_API __declspec(dllimport)
#endif

#include <stdint.h>

namespace PeakSearch
{
	struct PeakCondition
	{
		float thresholdRatio;
		float heightRatio;
	};

	struct Peak
	{
		int left;
		int right;
		int top;
		float peakValue;
	};

	PEAK_SEARTCH_API uint32_t createPeakSearchInstance(const PeakCondition* condition);
	PEAK_SEARTCH_API bool search(uint32_t handle, const float* array, size_t arraySize);
	PEAK_SEARTCH_API size_t getTopNPeak(uint32_t handle, Peak* peakBuffer, size_t bufferSize);
	PEAK_SEARTCH_API bool getMainPeak(uint32_t handle, Peak* result);
	PEAK_SEARTCH_API bool getSubPeak(uint32_t handle, float curLevel, Peak* result);
	PEAK_SEARTCH_API bool getNeighbourPeak(uint32_t handle, int curIdx, Peak* result, bool rfind);
	PEAK_SEARTCH_API int maxLevelPosition(uint32_t handle);
	PEAK_SEARTCH_API int minLevelPosition(uint32_t handle);
	PEAK_SEARTCH_API void destroyInstance(uint32_t handle);
}
