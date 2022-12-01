/**
* @file SampleOverlayProcessor.h

* @par Copyright (c):中电科第四十一研究所 装备事业部
* 版权所有，未经许可，不得复制、公开、转发

* @brief 样本叠加处理器。

* @author 何洪杰

* @date 2022年5月7日

* @note

* @version <1.0.0>

*/
#pragma once
#include "StreamConnect.h"
#include "node/detectionInternal.pb.h"

class HitsOverlap :public StreamInOut<detection::RawData, detection::RawData>
{
public:
	HitsOverlap(int numOverlap);
	virtual ~HitsOverlap();
public:
	void receive(detection::RawData& data) override;
	void onParamChange(uint32_t num);
private:
	void noOverlapProcess(detection::RawData&);
	void overlapProcess(detection::RawData&);
	void normalizeHits(detection::RawData&);	//hits值归一化
private:
	uint32_t numOverlap;
	uint32_t overlapCounter;
	std::unique_ptr<detection::RawData> overlapData;
};

