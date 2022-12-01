#include "pch.h"
#include "MergePolicy.h"

MergePolicyBase::MergePolicyBase(std::list<std::shared_ptr<DetectedSignal>>& hs)
	:historySignals(hs)
{

}

MergePolicyBase:: ~MergePolicyBase() = default;
