/**
* @file SampleOverlayProcessor.h

* @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
* ��Ȩ���У�δ����ɣ����ø��ơ�������ת��

* @brief �������Ӵ�������

* @author �κ��

* @date 2022��5��7��

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
	void normalizeHits(detection::RawData&);	//hitsֵ��һ��
private:
	uint32_t numOverlap;
	uint32_t overlapCounter;
	std::unique_ptr<detection::RawData> overlapData;
};

