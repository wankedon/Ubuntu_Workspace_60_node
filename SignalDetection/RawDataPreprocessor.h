/**
* @file DataPreparation.h

* @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
* ��Ȩ���У�δ����ɣ����ø��ơ�������ת��

* @brief ����Ԥ����Ԫ������hit���ӣ��޳����������������ݡ�

* @author �κ��

* @date 2022��5��9��

* @note

* @version <1.0.0>

*/
#pragma once

#include "node/detection.pb.h"
#include "node/detectionInternal.pb.h"

class HitsOverlap;
class HitsQualityFilter;
class DataFilter;
class RawDataPreprocessor
{
public:
	RawDataPreprocessor(const detection::SegmentationParam& defaultParameter, StreamIn<detection::RawData>& downstream);
	~RawDataPreprocessor();
public:
	void onParamChange(const detection::SegmentationParam& param);
	std::string addFilter(const detection::SegmentationFilterType& filter); //���ع�������guid
	void preprocess(detection::RawData& rawData);
private:
	detection::SegmentationParam parameter;
	std::unique_ptr<HitsOverlap> hitsOverlapProcessor;
	std::list<std::shared_ptr<DataFilter>> filters;
};

