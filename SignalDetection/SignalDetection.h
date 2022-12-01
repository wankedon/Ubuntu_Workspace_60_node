#pragma once

#include "node/detection.pb.h"
#include "node/detectionInternal.pb.h"

class SignalDetection
{
public:
	SignalDetection(); //�ɸ�����Ԫ�ṩ�����Ĭ�ϲ�����
	SignalDetection(const detection::DetectionParam& param);
	~SignalDetection();
public:
	void detect(detection::RawData& rawData); //�źż��
	void setDetectionParam(const detection::DetectionParam& params); //���в�������ͳһ�ӿڡ�
	void onCommitDetectedSignal(detection::SignalList& signalList); //�û��������������õĽӿڡ�
	void setDetailKeeping(const detection::DetailKeeping& detailKeeping);
private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

