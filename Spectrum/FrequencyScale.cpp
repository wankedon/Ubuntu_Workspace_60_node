/**
 * @file SweepZoomRange.cpp
 * @brief Ƶ���������
 * @author װ����ҵ������� ���� 
 * @version 0.1
 * @date 2020-12-09
 * 
 * @copyright Copyright (c) 2020  �й����ӿƼ����Ź�˾����ʮһ�о���
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
	if (curRange_.start_pos().segment_idx() == curRange_.stop_pos().segment_idx())	//��һ��
	{
		curRawPoints_ = curRange_.stop_pos().seg_internal_offset() - curRange_.start_pos().seg_internal_offset() + 1;
	}
	else
	{
		auto& segments = band_.segments();
		int firstSegIdx = curRange_.start_pos().segment_idx();
		curRawPoints_ = segments[firstSegIdx].points() - curRange_.start_pos().seg_internal_offset();	//��һ�εĵ���
		curRawPoints_ += (curRange_.stop_pos().seg_internal_offset() + 1);	//���һ�εĵ���
		for (auto segIdx = firstSegIdx + 1; segIdx < curRange_.stop_pos().segment_idx(); segIdx++)	//�ۼӸ��м�εĵ���
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