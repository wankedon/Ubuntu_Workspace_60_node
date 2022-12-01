/**
* @file Filters.h

* @par Copyright (c):中电科第四十一研究所 装备事业部
* 版权所有，未经许可，不得复制、公开、转发

* @brief 过滤器基类。

* @author 何洪杰

* @date 2022年5月9日

* @note

* @version <1.0.0>

*/
#pragma once

#include "StreamConnect.h"
#include "node/detection.pb.h"
#include "node/detectionInternal.pb.h"

class DataFilter :public StreamInOut<detection::RawData, detection::RawData>
{
public:
	DataFilter();
	virtual ~DataFilter();
public:
	void receive(detection::RawData& input) override; //重定义非const变量的receive函数。
	virtual void onParamChange(const detection::SegmentationParam& params) = 0; //设置过滤器参数。
protected:
	virtual void filter(detection::RawData& input) = 0; //进行过滤，各个过滤器的是具体实现方法。
};	

