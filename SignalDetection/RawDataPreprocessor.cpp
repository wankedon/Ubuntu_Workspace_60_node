#include "pch.h"
#include "StreamConnect.h"
#include "RawDataPreprocessor.h"
#include "HitsOverlap.h"
#include "DataFilter.h"
#include "MergeHelper.h"
#include "HitsSegmentationFilter.h"

using namespace std;

RawDataPreprocessor::RawDataPreprocessor(const detection::SegmentationParam& defaultParameter,StreamIn<detection::RawData>& downstream)
	:parameter(defaultParameter)
{
	//默认参数
	hitsOverlapProcessor = make_unique<HitsOverlap>(parameter.num_overlap()); //在此设置默认参数。
	filters.push_back(make_shared<HitsSegmentationFilter>(parameter.hits_param()));
	*hitsOverlapProcessor >> *filters.back() >> downstream;
}

RawDataPreprocessor::~RawDataPreprocessor() = default;

void RawDataPreprocessor::onParamChange(const detection::SegmentationParam& param)
{
	hitsOverlapProcessor->onParamChange(param.num_overlap());
	for (auto& filter : filters)
	{
		filter->onParamChange(param);
	}
}

string RawDataPreprocessor::addFilter(const detection::SegmentationFilterType& filter)
{
	//1 创建filter
	if (filter == detection::SegmentationFilterType::HITS) //根据类型在这里添加filter。
	{
		//已默认添加，后续可添加别
	}
	// add list
	// filterProcessors.push_back();
	//建立流
	//*hitsOverlayProcessor >> *filterProcessors.back() >> downstream;
	//swap()	
	return Helper::genUUID();
}

void RawDataPreprocessor::preprocess(detection::RawData& rawData)
{
	//叠加
	hitsOverlapProcessor->receive(rawData);
}

