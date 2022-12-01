/**
* @file OverlapStrategy.h

* @par Copyright (c):中电科第四十一研究所 装备事业部
* 版权所有，未经许可，不得复制、公开、转发

* @brief 融合策略基类(后续根据融合策略可依此进行拓展)

* @author 何洪杰

* @date 2022年4月29日

* @note

* @version <1.0.0>

*/

#pragma once

#include "node/detection.pb.h"
#include "node/detectionInternal.pb.h"

class DetectedSignal;
class MergePolicyBase
{
public:
	MergePolicyBase(std::list<std::shared_ptr<DetectedSignal>>& historySignals);
	virtual ~MergePolicyBase();
public:
	virtual void mergeFeature(detection::RawData& data) = 0; //增加历史信号列表中
	virtual void mergeSignal() = 0;
protected:
	std::list<std::shared_ptr<DetectedSignal>>& historySignals;
};

