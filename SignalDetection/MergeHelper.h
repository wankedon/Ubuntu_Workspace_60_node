/**
* @file OverlapStrategy.h

* @par Copyright (c):中电科第四十一研究所 装备事业部
* 版权所有，未经许可，不得复制、公开、转发

* @brief 融合过程中需要用到的一些计算工具函数，只提供计算，无需考虑状态信息。

* @author 何洪杰

* @date 2022年5月12日

* @note

* @version <1.0.0>

*/
#pragma once
#include "node/spectrum.pb.h"
#include "node/detection.pb.h"
#include "node/detectionInternal.pb.h"

namespace Helper
{
	const std::string genUUID();
	double intersection(const spectrum::FrequencySpan& spanA, const spectrum::FrequencySpan& spanB);
	double unionsection(const spectrum::FrequencySpan& spanA, const spectrum::FrequencySpan& spanB);
	bool isIntersected(const spectrum::SignalBand& lhs, const spectrum::SignalBand& rhs, float minIntersectionRatio);
	spectrum::SignalBand calMergeSigParam(const spectrum::FrequencySpan& nowFreq, const spectrum::FrequencySpan& newFreq);
	int64_t calTimeDiff(const Timestamp& pre, const Timestamp& now); //  计算时间差(ms)
	bool isBiggerThan(const Timestamp& pre, const Timestamp& now, int timeGapInMs); //  计算时间差(ms)
	StatisticVal initStatisticVal(double val);
	void updateStatisticVal(StatisticVal& statVal, size_t statCount, double now);
	void updateStatisticVal(StatisticVal& dst, const StatisticVal& src, size_t destStatCount, size_t srcStatCount);
	spectrum::FrequencySpan toFrequencySpan(const spectrum::SignalBand& signalBand);
	spectrum::SignalBand toCenterBandwidth(const spectrum::FrequencySpan& sp);

	template <class Iter> 
	StatisticVal doStatistic(Iter start, Iter stop)
	{
		auto sum = 0;
		auto minVal = *start;
		auto maxVal = *start;
		auto curVal = *start;
		auto iter = start;
		for (++iter; iter != stop; ++iter)
		{
			curVal = *iter;
			minVal = (curVal < minVal) ? curVal : minVal;
			maxVal = (curVal > maxVal) ? curVal : maxVal;
			sum += curVal;
		}
		StatisticVal result;
		result.set_cur(curVal);
		result.set_up(maxVal);
		result.set_down(minVal);
		result.set_mean(sum / (stop - start));
		return result;
	}
	std::list<detection::SignalFeature> extractSignalFeatures(const detection::RawData& data);
}
