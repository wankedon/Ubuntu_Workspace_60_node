#pragma once
/**
* @file SampleOverlayProcessor.h

* @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
* ��Ȩ���У�δ����ɣ����ø��ơ�������ת��

* @brief ���ݻ��崦������

* @author �κ��

* @date 2022��5��9��

* @note

* @version <1.0.0>

*/
#include "StreamConnect.h"
#include "node/detectionInternal.pb.h"
#include <future>

class DetectionDataBuffer : public StreamInOut<detection::RawData, detection::RawData>
{
public:
	DetectionDataBuffer();
	virtual ~DetectionDataBuffer();
public:
	void receive(detection::RawData& data) override;
private:
	void push(detection::RawData& data);
	void dispatchLoop();
private:
	std::list<detection::RawData> bufferList;
	std::mutex mtx;  //����
	std::future<void> processFuture;
	std::atomic_bool runFlag;
};

