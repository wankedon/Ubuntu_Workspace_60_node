/**
* @file HitsQualityFilter.h

* @par Copyright (c):中电科第四十一研究所 装备事业部
* 版权所有，未经许可，不得复制、公开、转发

* @brief 根据factor剔除不满足条件的hit值,根据设置的连续满足条件hit值的个数进行过滤。

* @author 何洪杰

* @date 2022年5月11日

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
	void filter(detection::RawData& input) override; //过滤
	void onParamChange(const detection::SegmentationParam& params) override; //根据用户的设置修改参数。
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

