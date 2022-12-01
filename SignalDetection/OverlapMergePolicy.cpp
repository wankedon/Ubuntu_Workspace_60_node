#include "pch.h"
#include "OverlapMergePolicy.h"
#include "DetectedSignal.h"
#include "MergeHelper.h"

using namespace std;
using namespace detection;

OverlapMergePolicy::OverlapMergePolicy(const detection::MergeParam& mp, std::list<std::shared_ptr<DetectedSignal>>& historySignals)
	:MergePolicyBase(historySignals),
	mergeParam(mp)
{
}

OverlapMergePolicy::~OverlapMergePolicy() = default;
/**
 * @brief 新信号特征与已存在信号特征进行频率合并。
 */
void OverlapMergePolicy::mergeFeature(detection::RawData& data)
{
	auto newFeatures = Helper::extractSignalFeatures(data);
	for (auto& feature : newFeatures)
	{
		auto sig = findFirstIntersectedSignal(feature);
		if (sig == nullptr)
		{
			sig = make_shared<DetectedSignal>();
			historySignals.push_back(sig);
		}
		sig->onNewFeature(std::move(feature), mergeParam.feature_merge_param().max_time_gap());
	}
}


shared_ptr<DetectedSignal> OverlapMergePolicy::findFirstIntersectedSignal(const detection::SignalFeature& feature)
{
	for (auto& sig : historySignals)
	{
		if (Helper::isIntersected(sig->signalBand(), feature.signal_band(), mergeParam.feature_merge_param().min_intersection_ratio()))
		{
			return sig;
		}
	}
	return nullptr;
}

void OverlapMergePolicy::mergeSignal()
{
	auto cur = historySignals.begin();
	while (cur != historySignals.end())
	{
		cur = mergeOneSignal(cur);
	}
}

std::list<std::shared_ptr<DetectedSignal>>::iterator OverlapMergePolicy::mergeOneSignal(std::list<std::shared_ptr<DetectedSignal>>::iterator cur)
{
	auto next = cur;
	++next;
	for(; next != historySignals.end(); ++next)
	{
		if (!Helper::isIntersected((*cur)->signalBand(), (*next)->signalBand(), mergeParam.signal_merge_param().min_intersection_ratio()))
			break;
		if (!Helper::isBiggerThan((*cur)->lastEmergeTime(), (*next)->lastEmergeTime(), mergeParam.signal_merge_param().max_time_gap()))
			(*cur)->mergeFrom(**next);
	}
	return next;
}

