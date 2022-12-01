#pragma once

#include "node/detection.pb.h"
#include "node/detectionInternal.pb.h"

class SignalDetection
{
public:
	SignalDetection(); //由各处理单元提供所需的默认参数。
	SignalDetection(const detection::DetectionParam& param);
	~SignalDetection();
public:
	void detect(detection::RawData& rawData); //信号检测
	void setDetectionParam(const detection::DetectionParam& params); //所有参数设置统一接口。
	void onCommitDetectedSignal(detection::SignalList& signalList); //用户请求数据所调用的接口。
	void setDetailKeeping(const detection::DetailKeeping& detailKeeping);
private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

