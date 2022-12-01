/**
* @file HitsQualityFilter.h

* @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
* ��Ȩ���У�δ����ɣ����ø��ơ�������ת��

* @brief ����factor�޳�������������hitֵ,�������õ�������������hitֵ�ĸ������й��ˡ�

* @author �κ��

* @date 2022��5��11��

* @note

* @version <1.0.0>

*/
#pragma once
#include "DataFilter.h"

class HitsSegmentationFilter : public DataFilter
{
public:
	HitsSegmentationFilter(const detection::HitsSegmentation& param);
	virtual ~HitsSegmentationFilter();
public:
	void filter(detection::RawData& input) override; //����
	void onParamChange(const detection::SegmentationParam& params) override; //�����û��������޸Ĳ�����
private:
	void excludeHits();
	void segmentation();
	void eraseGap();
	static bool hitInRange(float hit, const DRange& range);
	int spanToPoints(double span);
private:
	template <class Iter>
	bool findMatch(Iter& iter, Iter end, const std::function<bool(float val)>& functor)
	{
		bool find = false;
		while (iter != end)
		{
			find = functor(*iter);
			if(find)
				break;
			++iter;
		};
		return find;
	}
private:
	detection::RawData* curInput;
	detection::HitsSegmentation parameter;
};

