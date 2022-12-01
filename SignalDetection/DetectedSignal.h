/**
* @file OverlapStrategy.h

* @par Copyright (c):中电科第四十一研究所 装备事业部
* 版权所有，未经许可，不得复制、公开、转发

* @brief 信号融合处理类，每一个信号持有一个该类对象。

* @author 何洪杰

* @date 2022年4月29日

* @note

* @version <1.0.0>

*/
#pragma once

#include "node/detection.pb.h"
#include "node/detectionInternal.pb.h"

class DetectedSignal
{
public:
	DetectedSignal();
	~DetectedSignal();
public:
	std::string getId() const { return uuid; };
	void onNewFeature(detection::SignalFeature&& curFeature, int32_t timeFactor);
	void fillSignalList(detection::SignalList& signalList, int32_t timeFactor);
	void moveStagedToHistory();
	void mergeFrom(DetectedSignal& other);
	spectrum::SignalBand signalBand();
	Timestamp lastEmergeTime();
	Timestamp firstEmergeTime();
	detection::SignalActivity getActivity() const { return activity; }
	void setDetailKeeping(bool keep);
private:
	void update(detection::SignalSegment& dst, detection::SignalFeature&& curFeature);
	void update(detection::SignalSegment& dst, detection::SignalSegment& src);
	detection::SignalSegment newSignalSegment(detection::SignalFeature&& feature);
	detection::SignalSegment* lastStagedSegment();
	detection::SignalSegment* lastHistorySegment();
	int stagedHeaderSequence(int32_t timeFactor);
	bool prepareForFill(int32_t timeFactor);
private:  
	std::string uuid;
	bool keepDetail;
	detection::SignalActivity activity;
	std::list<detection::SignalSegment> staged; //只存在外部请求发送前的所有更新的信号。发送完成后清空。
	std::list<detection::SignalSegment> history;    //历史更新所有信息。
};

