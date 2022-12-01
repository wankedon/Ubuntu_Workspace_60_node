/**
 * @file SweepZoomRange.cpp
 * @brief 频谱区间管理
 * @author 装备事业部软件组 杨青 
 * @version 0.1
 * @date 2020-12-09
 * 
 * @copyright Copyright (c) 2020  中国电子科技集团公司第四十一研究所
 * 
 */
#include "pch.h"
#include "FrequencyScale.h"
#include "SpectrumUtil.h"

using namespace std;

FrequencyScale::FrequencyScale(const FrequencyBand& whole)
	:band_(whole),
	totalRawPoints_(frequencyBandPoints(whole)),
	wholeSpan_(bandSpan(whole)),
	wholeRange_(bandRange(whole)),
	curRawPoints_(0)
{
	resetCurrent(wholeSpan_);
}

FrequencyScale::~FrequencyScale(void)
{
}

FrequencySpan FrequencyScale::adjustSpan(const FrequencySpan& range)
{
	const double minFreqSpan = freqStep() * 3;
	auto result = range;
	if (result.start_freq() < wholeSpan_.start_freq())
		result.set_start_freq(wholeSpan_.start_freq());
	if (result.stop_freq() > wholeSpan_.stop_freq())
		result.set_stop_freq(wholeSpan_.stop_freq());
	if (result.stop_freq() < result.start_freq() + minFreqSpan)
		result.set_stop_freq(result.start_freq() + minFreqSpan);
	return result;
}

bool FrequencyScale::isLastSegment(int32_t segIdx) const
{
	auto segPos = segmentPosInRange(segIdx, wholeRange_);
	return (segPos == SegPosition::MULTI_LAST || segPos == SegPosition::UNIQUE);
}

double FrequencyScale::freqStep() const
{
	return ::freqStep(band_.segments()[0]);
}

void FrequencyScale::resetCurrent(const FrequencySpan& span)
{
	curSpan_ = adjustSpan(span);
	curRange_ = *frequencySpanToRange(curSpan_, band_);
	if (curRange_.start_pos().segment_idx() == curRange_.stop_pos().segment_idx())	//就一段
	{
		curRawPoints_ = curRange_.stop_pos().seg_internal_offset() - curRange_.start_pos().seg_internal_offset() + 1;
	}
	else
	{
		auto& segments = band_.segments();
		int firstSegIdx = curRange_.start_pos().segment_idx();
		curRawPoints_ = segments[firstSegIdx].points() - curRange_.start_pos().seg_internal_offset();	//第一段的点数
		curRawPoints_ += (curRange_.stop_pos().seg_internal_offset() + 1);	//最后一段的点数
		for (auto segIdx = firstSegIdx + 1; segIdx < curRange_.stop_pos().segment_idx(); segIdx++)	//累加各中间段的点数
		{
			curRawPoints_ += segments[segIdx].points();
		}
	}
}

void FrequencyScale::onSpanChange(const FrequencySpan& range)
{
	resetCurrent(range);
}

std::unique_ptr<FrequencyRange> FrequencyScale::calcRange(const FrequencySpan& span) const
{
	return frequencySpanToRange(span, band_);
}