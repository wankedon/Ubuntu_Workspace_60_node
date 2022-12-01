/**
* @file SignalMerge.h

* @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
* ��Ȩ���У�δ����ɣ����ø��ơ�������ת��

* @brief �źźϲ�����Ԫ

* @author �κ��

* @date 2022��4��27��

* @note �����ɵ�emissions������ȴ����˹���ÿһ֡�ź��б��е��ź�Ƭ�ζ�Ҫ������һ֡���ź��б��е��ź�Ƭ�ν����ںϴ���,
        ����Ƶ���ںϣ�emission֮�䣩��ʱ���ںϣ�ͬһemission�������档

* @version <1.0.0>

*/
#pragma once

#include "node/detection.pb.h"
#include "node/detectionInternal.pb.h"

class MergePolicyBase;
class DetectedSignal;
class MergePolicyManager 
{
public:
	MergePolicyManager(std::list<std::shared_ptr<DetectedSignal>>& historySignals);
	~MergePolicyManager();
public:
	std::unique_ptr<MergePolicyBase> createPolicy(const detection::MergeParam& param);
private:
	std::list<std::shared_ptr<DetectedSignal>>& historySignals;
};