/**
* @file OverlapStrategy.h

* @par Copyright (c):中电科第四十一研究所 装备事业部
* 版权所有，未经许可，不得复制、公开、转发

* @brief 频率重叠策略实现

* @author 何洪杰

* @date 2022年4月29日

* @note

* @version <1.0.0>

*/
#pragma once
#include "MergePolicy.h"
class OverlapMergePolicy :public MergePolicyBase
{
public:
	OverlapMergePolicy(const detection::MergeParam& params, std::list<std::shared_ptr<DetectedSignal>>& historySignals);
	virtual ~OverlapMergePolicy();
public:
	void mergeFeature(detection::RawData& data) override;
	void mergeSignal() override;
private:
	TimeSpan getTimeSpan(detection::RawData& data) const { return data.header().time_span(); };
	std::shared_ptr<DetectedSignal> findFirstIntersectedSignal(const detection::SignalFeature& feature);
	std::list<std::shared_ptr<DetectedSignal>>::iterator mergeOneSignal(std::list<std::shared_ptr<DetectedSignal>>::iterator iter);
private:
	const detection::MergeParam& mergeParam;
};

