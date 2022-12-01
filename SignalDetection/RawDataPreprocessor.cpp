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
	//Ĭ�ϲ���
	hitsOverlapProcessor = make_unique<HitsOverlap>(parameter.num_overlap()); //�ڴ�����Ĭ�ϲ�����
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
	//1 ����filter
	if (filter == detection::SegmentationFilterType::HITS) //�����������������filter��
	{
		//��Ĭ����ӣ���������ӱ�
	}
	// add list
	// filterProcessors.push_back();
	//������
	//*hitsOverlayProcessor >> *filterProcessors.back() >> downstream;
	//swap()	
	return Helper::genUUID();
}

void RawDataPreprocessor::preprocess(detection::RawData& rawData)
{
	//����
	hitsOverlapProcessor->receive(rawData);
}

