#include "pch.h"
#include "SpectrumUtil.h"

using namespace spectrum;
using namespace std;

int frequencyBandPoints(const spectrum::FrequencyBand& band)
{
	int totoalPoints = 0;
	for (auto& seg : band.segments())
	{
		totoalPoints += seg.points();
	}
	return totoalPoints;
}

spectrum::FrequencySpan bandSpan(const spectrum::FrequencyBand& band)
{
	spectrum::FrequencySpan span;
	span.set_start_freq(band.segments().begin()->freq_span().start_freq());
	span.set_stop_freq(band.segments().rbegin()->freq_span().stop_freq());
	return span;
}

spectrum::FrequencyRange bandRange(const spectrum::FrequencyBand& band)
{
	spectrum::FrequencyRange range;
	range.mutable_start_pos()->set_segment_idx(0);
	range.mutable_start_pos()->set_seg_internal_offset(0);
	range.mutable_stop_pos()->set_segment_idx(band.segments_size() - 1);
	range.mutable_stop_pos()->set_seg_internal_offset(band.segments().rbegin()->points() - 1);
	return range;
}

double freqStep(const spectrum::FrequencySegment& freqSeg)
{
	if (freqSeg.points() == 0)
		return 0;
	const double start = freqSeg.freq_span().start_freq();
	const double stop = freqSeg.freq_span().stop_freq();
	return (stop - start) / (freqSeg.points() - 1);
}

std::unique_ptr<int32_t> freqToIndex(const spectrum::FrequencySegment& freqSeg, const double frequency)
{
	const double start = freqSeg.freq_span().start_freq();
	const double stop = freqSeg.freq_span().stop_freq();
	if (((frequency - start) < -0.01) || ((frequency - stop) > 0.01))
		return nullptr;
	auto idx = static_cast<int32_t>((frequency - start) / freqStep(freqSeg));
	if (idx > (freqSeg.points() - 1))
		idx = freqSeg.points() - 1;
	return make_unique<int32_t>(idx);
}

std::unique_ptr<double> indexToFreq(const spectrum::FrequencySegment& freqSeg, int32_t idx)
{
	if (idx >= freqSeg.points())
		return nullptr;
	return make_unique<double>(freqSeg.freq_span().start_freq() + freqStep(freqSeg) * idx);
}

std::unique_ptr<double> posToFrequency(const FrequencyPos pos, const FrequencyBand& band)
{
	if (pos.segment_idx() >= band.segments_size())
		return nullptr;
	return indexToFreq(band.segments()[pos.segment_idx()], pos.seg_internal_offset());
}

std::unique_ptr<FrequencyPos> frequencyToPos(double frequency, const FrequencyBand& band)
{
	int32_t segIdx = 0;
	std::unique_ptr<FrequencyPos> result;
	for (auto& seg : band.segments())
	{
		auto findResult(freqToIndex(seg, frequency));
		if (findResult)
		{
			result = make_unique<FrequencyPos>();
			result->set_segment_idx(segIdx);
			result->set_seg_internal_offset(*findResult);
			break;
		}
		else
		{
			segIdx++;
		}
	}
	return result;
}

std::unique_ptr<spectrum::FrequencyRange> frequencySpanToRange(const spectrum::FrequencySpan span, const spectrum::FrequencyBand& band)
{
	auto start = frequencyToPos(span.start_freq(), band);
	auto stop = frequencyToPos(span.stop_freq(), band);
	if (start && stop)
	{
		auto result = std::unique_ptr<spectrum::FrequencyRange>();
		*result->mutable_start_pos() = *start;
		*result->mutable_stop_pos() = *stop;
		return result;
	}
	else
	{
		return nullptr;
	}
}

std::unique_ptr<spectrum::FrequencyRange> frequencySpanToRange(const spectrum::FrequencySpan& span, const spectrum::FrequencyBand& band)
{
	auto start = frequencyToPos(span.start_freq(), band);
	auto stop = frequencyToPos(span.stop_freq(), band);
	if (start == nullptr || stop == nullptr)
		return nullptr;
	auto result = make_unique<spectrum::FrequencyRange>();
	*result->mutable_start_pos() = *start;
	*result->mutable_stop_pos() = *stop;
	return result;
}

SegPosition segmentPosInRange(int segIdx, const spectrum::FrequencyRange& range)
{
	if (segIdx < range.start_pos().segment_idx() || segIdx > range.stop_pos().segment_idx())
		return SegPosition::OUT_OF_RANGE;
	else
	{
		if (segIdx == range.start_pos().segment_idx())
		{
			if (segIdx != range.stop_pos().segment_idx())
			{
				return SegPosition::MULTI_FIRST;
			}
			else
			{
				return SegPosition::UNIQUE;
			}
		}
		else if (segIdx == range.stop_pos().segment_idx())
			return SegPosition::MULTI_LAST;
		else
			return SegPosition::MULTI_MID;
	}
}

std::unique_ptr<std::pair<int, int>> segmentPartInRange(int32_t segIdx, const spectrum::FrequencyRange& range, const spectrum::FrequencyBand& band)
{
	auto segPos = segmentPosInRange(segIdx, range);
	if (segPos == SegPosition::OUT_OF_RANGE)
		return nullptr;
	auto result = make_unique<std::pair<int, int>>();
	if (segPos == SegPosition::UNIQUE)	//¾ÍÒ»¶Î
	{
		result->first = range.start_pos().seg_internal_offset();
		result->second = range.stop_pos().seg_internal_offset();
	}
	else
	{
		auto& segments = band.segments();
		if (segPos == SegPosition::MULTI_FIRST)
		{
			result->first = range.start_pos().seg_internal_offset();
			result->second = segments[segIdx].points() - 1;
		}
		else if (segPos == SegPosition::MULTI_LAST)
		{
			result->first = 0;
			result->second = range.stop_pos().seg_internal_offset();
		}
		else
		{
			result->first = 0;
			result->second = segments[segIdx].points() - 1;
		}
	}
	return result;
}

double intersection(const spectrum::FrequencySpan& spanA, const spectrum::FrequencySpan& spanB)
{
	auto intersectionStart = fmax(spanA.start_freq(), spanB.start_freq());
	auto intersectionStop = fmin(spanA.stop_freq(), spanB.stop_freq());
	auto diff = intersectionStop - intersectionStart;
	return diff > 0 ? diff : 0;
}

double unionsection(const spectrum::FrequencySpan& spanA, const spectrum::FrequencySpan& spanB)
{
	auto unionStart = fmin(spanA.start_freq(), spanB.start_freq());
	auto unionStop = fmax(spanA.stop_freq(), spanB.stop_freq());
	auto maxSpanSum = spanA.stop_freq() + spanB.stop_freq() - spanA.start_freq() - spanB.start_freq();
	auto sum = (unionStop - unionStart);
	return sum < maxSpanSum ? sum : maxSpanSum;
}