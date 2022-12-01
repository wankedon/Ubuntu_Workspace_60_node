#include "pch.h"
#include "MergePolicyManager.h"
#include "OverlapMergePolicy.h"

using namespace std;
using namespace detection;

MergePolicyManager::MergePolicyManager(std::list<std::shared_ptr<DetectedSignal>>& hs)
	:historySignals(hs)
{

}

MergePolicyManager::~MergePolicyManager() = default;


std::unique_ptr<MergePolicyBase> MergePolicyManager::createPolicy(const detection::MergeParam& param)
{
	//目前仅返回OverlapMergePolicy
	if (param.policy() == SPECTRUM_INTERSECT_POLICY)
	{
		return make_unique<OverlapMergePolicy>(param, historySignals);
	}
	return nullptr;
}
