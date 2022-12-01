/**
* @file SignalMerge.h

* @par Copyright (c):中电科第四十一研究所 装备事业部
* 版权所有，未经许可，不得复制、公开、转发

* @brief 信号合并处理单元

* @author 何洪杰

* @date 2022年4月27日

* @note 将生成的emissions进行深度处理，此过程每一帧信号列表中的信号片段都要跟其上一帧的信号列表中的信号片段进行融合处理,
        包括频率融合（emission之间）和时间融合（同一emission）两方面。

* @version <1.0.0>

*/
#pragma once

#include "node/detection.pb.h"
#include "node/detectionInternal.pb.h"

class MergePolicyBase;
class DetectedSignal;
class MergePolicyManager 
{
public:
	MergePolicyManager(std::list<std::shared_ptr<DetectedSignal>>& historySignals);
	~MergePolicyManager();
public:
	std::unique_ptr<MergePolicyBase> createPolicy(const detection::MergeParam& param);
private:
	std::list<std::shared_ptr<DetectedSignal>>& historySignals;
};