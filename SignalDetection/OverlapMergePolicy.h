/**
* @file OverlapStrategy.h

* @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
* ��Ȩ���У�δ����ɣ����ø��ơ�������ת��

* @brief Ƶ���ص�����ʵ��

* @author �κ��

* @date 2022��4��29��

* @note

* @version <1.0.0>

*/
#pragma once
#include "MergePolicy.h"
class OverlapMergePolicy :public MergePolicyBase
{
public:
	OverlapMergePolicy(const detection::MergeParam& params, std::list<std::shared_ptr<DetectedSignal>>& historySignals);
	virtual ~OverlapMergePolicy();
public:
	void mergeFeature(detection::RawData& data) override;
	void mergeSignal() override;
private:
	TimeSpan getTimeSpan(detection::RawData& data) const { return data.header().time_span(); };
	std::shared_ptr<DetectedSignal> findFirstIntersectedSignal(const detection::SignalFeature& feature);
	std::list<std::shared_ptr<DetectedSignal>>::iterator mergeOneSignal(std::list<std::shared_ptr<DetectedSignal>>::iterator iter);
private:
	const detection::MergeParam& mergeParam;
};

