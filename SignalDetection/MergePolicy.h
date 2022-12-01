/**
* @file OverlapStrategy.h

* @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
* ��Ȩ���У�δ����ɣ����ø��ơ�������ת��

* @brief �ںϲ��Ի���(���������ںϲ��Կ����˽�����չ)

* @author �κ��

* @date 2022��4��29��

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
	virtual void mergeFeature(detection::RawData& data) = 0; //������ʷ�ź��б���
	virtual void mergeSignal() = 0;
protected:
	std::list<std::shared_ptr<DetectedSignal>>& historySignals;
};

