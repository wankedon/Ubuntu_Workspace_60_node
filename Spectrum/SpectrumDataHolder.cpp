/**
 * @file SpectrumDataHolder.cpp
 * @brief 频谱数据保持
 * @author 装备事业部软件组 杨青 
 * @version 0.1
 * @date 2021-9-30
 * 
 * @copyright Copyright (c) 2021  中国电子科技集团公司第四十一研究所
 * 
 */
#include "pch.h"
#include "SpectrumDataHolder.h"
#include "SpectrumDetector.h"
#include "TraceDataHolder.h"

using namespace std;
using namespace spectrum;

Detector::DetectType detectType(spectrum::DataHoldType dhType)
{
	if (dhType == spectrum::MIN_HOLD)
	{
		return Detector::DetectType::NEGAPEAK;
	}
	else
	{
		return Detector::DetectType::POSIPEAK;
	}
}

SpectrumDataHolder::SpectrumDataHolder(spectrum::DataHoldType holdType)
	:DH_TYPE(holdType),
	dataHolder(TraceDataHolder::makeTraceDataHolder(holdType))
{

}

SpectrumDataHolder::~SpectrumDataHolder() = default;

void SpectrumDataHolder::forwardData()
{
	StreamOut::send(*dataHolder);
}

void SpectrumDataHolder::clear()
{
	dataHolder->clear();
}

void SpectrumDataHolder::inputSegData(int32_t segIdx, const float* data, size_t count)
{
	dataHolder->input(segIdx, data, count);
}

void SpectrumDataHolder::loadSweepData(const spectrum::FrequencyBand& band, const std::vector<char>& sweepData)
{
	const float* ptr = (const float*)sweepData.data();
	int i = 0;
	for (auto& seg : band.segments())
	{
		inputSegData(i, ptr, seg.points());
		ptr += seg.points();
		++i;
	}
}

size_t SpectrumDataHolder::segmentCount() const
{
	return dataHolder->segmentCount();
}

std::shared_ptr<SegmentData> SpectrumDataHolder::getSegmentData(int32_t segIdx) const
{
	return dataHolder->getSegmentData(segIdx);
}

SegmentData SpectrumDataHolder::getPanorama(size_t expectedOutput)
{
	SegmentData raw(dataHolder->integrate());
	auto buffer = tryDetect(panoramaDetector, detectType(DH_TYPE), raw, expectedOutput);
	if (buffer.ptr != nullptr && buffer.size != 0)
	{
		return SegmentData{ buffer.ptr, buffer.ptr + buffer.size };
	}
	else
	{
		return raw;
	}
}

SegmentData SpectrumDataHolder::getCloseshot(size_t expectedOutput, const FrequencyScale& scale)
{
	SegmentData zoominPart(dataHolder->getZoominPart(scale));
	auto buffer = tryDetect(zoomInDetector, detectType(DH_TYPE), zoominPart, expectedOutput);
	if (buffer.ptr != nullptr && buffer.size != 0)
	{
		return SegmentData{ buffer.ptr, buffer.ptr + buffer.size };
	}
	else
	{
		return zoominPart;
	}
}