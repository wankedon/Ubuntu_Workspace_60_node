/**
* @file OverlapStrategy.h

* @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
* ��Ȩ���У�δ����ɣ����ø��ơ�������ת��

* @brief �ź��ںϴ����࣬ÿһ���źų���һ���������

* @author �κ��

* @date 2022��4��29��

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
	std::list<detection::SignalSegment> staged; //ֻ�����ⲿ������ǰ�����и��µ��źš�������ɺ���ա�
	std::list<detection::SignalSegment> history;    //��ʷ����������Ϣ��
};

