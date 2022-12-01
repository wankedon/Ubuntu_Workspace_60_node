/**
* @file DataPreparation.h

* @par Copyright (c):中电科第四十一研究所 装备事业部
* 版权所有，未经许可，不得复制、公开、转发

* @brief 数据预处理单元，包括hit叠加，剔除，过滤三部分内容。

* @author 何洪杰

* @date 2022年5月9日

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
	std::string addFilter(const detection::SegmentationFilterType& filter); //返回过滤器的guid
	void preprocess(detection::RawData& rawData);
private:
	detection::SegmentationParam parameter;
	std::unique_ptr<HitsOverlap> hitsOverlapProcessor;
	std::list<std::shared_ptr<DataFilter>> filters;
};

