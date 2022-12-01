#pragma once
/**
* @file SampleOverlayProcessor.h

* @par Copyright (c):中电科第四十一研究所 装备事业部
* 版权所有，未经许可，不得复制、公开、转发

* @brief 数据缓冲处理器。

* @author 何洪杰

* @date 2022年5月9日

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
	std::mutex mtx;  //互斥
	std::future<void> processFuture;
	std::atomic_bool runFlag;
};

