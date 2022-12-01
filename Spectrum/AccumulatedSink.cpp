/**
 * @file AccumulatedSink.cpp
 * @brief 数据累积
 * @author 装备事业部软件组 杨青 
 * @version 0.1
 * @date 2021-9-30
 * 
 * @copyright Copyright (c) 2021  中国电子科技集团公司第四十一研究所
 * 
 */
#include "pch.h"
#include "AccumulatedSink.h"
#include "TraceDataHolder.h"

using namespace std;

AccumulatedSink::AccumulatedSink()
	:dataCache(TraceDataHolder::makeTraceDataHolder(spectrum::DataHoldType::NO_HOLD)),
	enabled(false)
{

}

AccumulatedSink::~AccumulatedSink() = default;

void AccumulatedSink::receive(const TraceDataHolder & data)
{
	if (enabled)
		*dataCache += data;
}

void AccumulatedSink::enable()
{
	enabled = true;
}

void AccumulatedSink::disable()
{
	enabled = false;
	dataCache->clear();
}

void AccumulatedSink::reset()
{
	dataCache->clear();
}

void AccumulatedSink::fillDataBuffer(vector<char>&buffer)
{
	auto src = dataCache->integrate();
	size_t srcBytes = src.size() * sizeof(float);
	if (buffer.size() != srcBytes)
	{
		buffer.resize(srcBytes);
	}
	memcpy(buffer.data(), src.data(), srcBytes);
}

void AccumulatedSink::fillDataBuffer(vector<float>& buffer)
{
	auto src = dataCache->integrate();
	size_t srcBytes = src.size();
	if (buffer.size() != srcBytes)
	{
		buffer.resize(srcBytes);
	}
	memcpy(buffer.data(), src.data(), srcBytes);
}
