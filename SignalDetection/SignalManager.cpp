#include "pch.h"
#include "SignalManager.h"
#include "DetectedSignal.h"
#include "MergePolicy.h"
#include "MergePolicyManager.h"

using namespace std;

SignalManager::SignalManager(const detection::DetectionParam& defaultParam)
	:mergeParam(defaultParam.merge_param()),
	commitPeriod(1),
	mergeCounter(0)
{
	if (defaultParam.options().has_commit_result_period())
	{
		commitPeriod = std::max((int64_t)1, defaultParam.options().commit_result_period().value());
	}
	policyManager = make_unique<MergePolicyManager>(historySignals);
}

SignalManager::~SignalManager() = default;

void SignalManager::receive(detection::RawData& data)
{
	auto policy = policyManager->createPolicy(mergeParam);
	if (policy != nullptr)
	{
		policy->mergeFeature(data);
		++mergeCounter;
	}
}

void SignalManager::fillSignalList(detection::SignalList& signalList)
{
	for (const auto& sig : historySignals)
	{
		sig->fillSignalList(signalList, mergeParam.feature_merge_param().max_time_gap());
	}
}

void SignalManager::moveStagedToHistory()
{
	for (const auto& sig : historySignals)
	{
		sig->moveStagedToHistory();
	}
}

list<string> SignalManager::mergeSignal()
{	
	auto policy = policyManager->createPolicy(mergeParam);
	if (policy != nullptr)
	{
		policy->mergeSignal();
	}
	list<string> mergedSignalsId;
	for (auto iter = historySignals.begin(); iter != historySignals.end();)
	{
		if ((*iter)->getActivity() == detection::MERGED)
		{
			mergedSignalsId.push_back((*iter)->getId());
			mergedSignals.push_back(*iter);
			iter = historySignals.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	return mergedSignalsId;
}

void SignalManager::onCommitDetectedSignal(detection::SignalList& signalList)
{
	if (mergeCounter % commitPeriod != 0)	//还没有到提交时机，不做处理
		return;
	//先对历史信号进行排序
	historySignals.sort([](const std::shared_ptr<DetectedSignal>& lhs, const std::shared_ptr<DetectedSignal>& rhs)
		{
			return lhs->signalBand().center_frequency() < rhs->signalBand().center_frequency();
		});
	fillSignalList(signalList);		
	moveStagedToHistory();	
	auto src= mergeSignal();
	auto dst = signalList.mutable_merged_signal_id();
	*dst = { src.begin(),src.end() };
}

void SignalManager::onParamChange(const detection::DetectionParam& param)
{
	if (param.has_merge_param())
	{
		mergeParam = param.merge_param();
	}
	if (param.has_options())
	{
		if (param.options().has_clear_signal_lib_history() && param.options().clear_signal_lib_history().value())
		{
			clear();
		}
		if (param.options().has_commit_result_period())
		{
			commitPeriod = std::max((int64_t)1, param.options().commit_result_period().value());
		}
	}
}

void SignalManager::setSignalKeeping(const detection::DetailKeeping& detailKeeping)
{
	bool keeping = detailKeeping.keeping();
	for (auto& id : detailKeeping.signal_id())
	{
		for (auto& sig : historySignals)
		{
			if (sig->getId() == id)
			{
				sig->setDetailKeeping(keeping);
				break;
			}
		}
	}

}

void SignalManager::clear()
{
	historySignals.clear();
	mergedSignals.clear();
	mergeCounter = 0;
}